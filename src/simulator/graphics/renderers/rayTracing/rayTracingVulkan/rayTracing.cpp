//--------------------------------------------------
// Atta Ray Tracing Vulkan
// rayTracing.cpp
// Date: 2020-07-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "rayTracing.h"
#include <chrono>
#include "simulator/graphics/core/vertex.h"
#include "simulator/graphics/vulkan/imageMemoryBarrier.h"
#include "simulator/graphics/vulkan/stagingBuffer.h"
#include "simulator/helpers/log.h"
#include "simulator/helpers/evaluator.h"

namespace atta::rt::vk
{
	RayTracing::RayTracing(CreateInfo info):
		Renderer({info.vkCore, info.width, info.height, info.viewMat, RENDERER_TYPE_RAY_TRACING_VULKAN}), 
		_scene(info.scene)
	{
		_device = _vkCore->getDevice();
		_commandPool = _vkCore->getCommandPool();

		_deviceProcedures = std::make_shared<DeviceProcedures>(_device);
		_rayTracingProperties  = std::make_shared<RayTracingProperties>(_device);
		_imageExtent = {info.width, info.height};
		_imageFormat = VK_FORMAT_R32G32B32A32_SFLOAT;

		// Create uniform buffer
		_uniformBuffer = std::make_shared<rt::vk::UniformBuffer>(_vkCore->getDevice());
		rt::vk::UniformBufferObject ubo;
		ubo.viewMat = info.viewMat;
		ubo.projMat = info.projMat;
		ubo.projMat.data[5] *= -1;
		ubo.viewMatInverse = atta::inverse(ubo.viewMat);
		ubo.projMatInverse = atta::inverse(ubo.projMat);
		ubo.samplesPerFrame = 8;
		ubo.totalNumberOfSamples = 0;// The total is increased every render() call
		ubo.numberOfBounces = 8;
		_uniformBuffer->setValue(ubo);

		// Create ray tracing specific
		createAccelerationStructures();
		createAccumulationImage();
		createPipeline();

		// Change output image layout
		//VkCommandBuffer commandBuffer = _commandPool->beginSingleTimeCommands();
		//{
		//	VkImageSubresourceRange subresourceRange;
		//	subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		//	subresourceRange.baseMipLevel = 0;
		//	subresourceRange.levelCount = 1;
		//	subresourceRange.baseArrayLayer = 0;
		//	subresourceRange.layerCount = 1;

		//	atta::vk::ImageMemoryBarrier::insert(commandBuffer, _image->handle(), subresourceRange, VK_ACCESS_TRANSFER_WRITE_BIT,
		//		0, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
		//}
		//_commandPool->endSingleTimeCommands(commandBuffer);
	}

	RayTracing::~RayTracing()
	{
	}

	void RayTracing::createPipeline()
	{
		_rayTracingPipeline = std::make_shared<RayTracingPipeline>(
				_device, _deviceProcedures, 
				_tlas[0], _accumulationImageView, _imageView, _uniformBuffer, _vkCore);

		const std::vector<ShaderBindingTable::Entry> rayGenPrograms = { {_rayTracingPipeline->getRayGenShaderIndex(), {}} };
		const std::vector<ShaderBindingTable::Entry> missPrograms = { {_rayTracingPipeline->getMissShaderIndex(), {}} };
		const std::vector<ShaderBindingTable::Entry> hitGroups = { {_rayTracingPipeline->getTriangleHitGroupIndex(), {}}, {_rayTracingPipeline->getProceduralHitGroupIndex(), {}}};

		_shaderBindingTable = std::make_shared<ShaderBindingTable>(_device, _deviceProcedures, _rayTracingPipeline, _rayTracingProperties, rayGenPrograms, missPrograms, hitGroups);
	}

	void RayTracing::createAccelerationStructures()
	{
		Log::info("rt::vk::RayTracing", "Building acceleration structures...");
		LocalEvaluator eval;

		VkCommandBuffer commandBuffer = _commandPool->beginSingleTimeCommands();
		{
			createBottomLevelStructures(commandBuffer);
			createTopLevelStructures(commandBuffer);
		}
		_commandPool->endSingleTimeCommands(commandBuffer);

		eval.stop();
		Log::info("rt::vk::RayTracing", "Finished: [w]$0ms", eval.getMs());
	}

	void RayTracing::createAccumulationImage()
	{
		const auto tiling = VK_IMAGE_TILING_OPTIMAL;

		_accumulationImage = std::make_shared<atta::vk::Image>(_device, 
				_imageExtent.width, _imageExtent.height, _imageFormat, 
				tiling, VK_IMAGE_USAGE_STORAGE_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		_accumulationImageView = std::make_shared<atta::vk::ImageView>(_device, _accumulationImage->handle(), _imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
	}

	void RayTracing::render(VkCommandBuffer commandBuffer)
	{
		// Increate total number of samples
		rt::vk::UniformBufferObject ubo = _uniformBuffer->getValue();
		ubo.totalNumberOfSamples += ubo.samplesPerFrame;
		_uniformBuffer->setValue(ubo);

		// Prepare for rendering
		VkDescriptorSet descriptorSets[] = { _rayTracingPipeline->getDescriptorSet(0) };

		VkImageSubresourceRange subresourceRange;
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.baseArrayLayer = 0;
		subresourceRange.layerCount = 1;

		// Acquire destination images for rendering
		atta::vk::ImageMemoryBarrier::insert(commandBuffer, _accumulationImage->handle(), subresourceRange, 0,
			VK_ACCESS_SHADER_WRITE_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);

		atta::vk::ImageMemoryBarrier::insert(commandBuffer, _image->handle(), subresourceRange, 0,
			VK_ACCESS_SHADER_WRITE_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, _rayTracingPipeline->handle());
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, _rayTracingPipeline->getPipelineLayout()->handle(), 0, 1, descriptorSets, 0, nullptr);

		// Describe the shader binding table.
		VkStridedDeviceAddressRegionKHR raygenShaderBindingTable = {};
		raygenShaderBindingTable.deviceAddress = _shaderBindingTable->rayGenDeviceAddress();
		raygenShaderBindingTable.stride = _shaderBindingTable->rayGenEntrySize();
		raygenShaderBindingTable.size = _shaderBindingTable->rayGenSize();

		VkStridedDeviceAddressRegionKHR missShaderBindingTable = {};
		missShaderBindingTable.deviceAddress = _shaderBindingTable->missDeviceAddress();
		missShaderBindingTable.stride = _shaderBindingTable->missEntrySize();
		missShaderBindingTable.size = _shaderBindingTable->missSize();

		VkStridedDeviceAddressRegionKHR hitShaderBindingTable = {};
		hitShaderBindingTable.deviceAddress = _shaderBindingTable->hitGroupDeviceAddress();
		hitShaderBindingTable.stride = _shaderBindingTable->hitGroupEntrySize();
		hitShaderBindingTable.size = _shaderBindingTable->hitGroupSize();

		VkStridedDeviceAddressRegionKHR callableShaderBindingTable = {};

		// Ray tracing to output/accumulation image
		_deviceProcedures->vkCmdTraceRaysKHR(commandBuffer,
			&raygenShaderBindingTable, &missShaderBindingTable, &hitShaderBindingTable, &callableShaderBindingTable,
			_imageExtent.width, _imageExtent.height, 1);

		// Change output image format to the expected by the workerGui thread
		atta::vk::ImageMemoryBarrier::insert(commandBuffer, _image->handle(), subresourceRange, VK_ACCESS_TRANSFER_WRITE_BIT,
			0, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	}

	void RayTracing::createBottomLevelStructures(VkCommandBuffer commandBuffer)
	{
		// Bottom level acceleration structure
		// Triangles via vertex buffers. Procedurals via AABBs.
		unsigned vertexOffset = 0;
		unsigned indexOffset = 0;
		uint32_t aabbOffset = 0;

		// Create blas object for each model
		for(Mesh* mesh : Mesh::allMeshes)
		{
			if(mesh==nullptr)
				continue;
			const uint32_t vertexCount = mesh->getVerticesSize();
			const uint32_t indexCount = mesh->getIndicesSize();

			std::shared_ptr<BottomLevelGeometry> geometries = std::make_shared<BottomLevelGeometry>();

			//false// getProcedural
			//	? geometries.AddGeometryAabb(_vkCore, aabbOffset, 1, true)
			geometries->addGeometry(_vkCore, vertexOffset, vertexCount, indexOffset, indexCount, true);

			std::shared_ptr<BottomLevelAccelerationStructure> blas = std::make_shared<BottomLevelAccelerationStructure>(_deviceProcedures, _rayTracingProperties, geometries);
			_blas.push_back(blas);

			vertexOffset += vertexCount*sizeof(Vertex);
			indexOffset += indexCount*sizeof(uint32_t);
			aabbOffset += sizeof(VkAabbPositionsKHR);
		}

		// Calculate total memory size to allocate
		VkAccelerationStructureBuildSizesInfoKHR total = getTotalRequirements(_blas);

		_bottomBuffer = std::make_shared<atta::vk::Buffer>(
				_device, total.accelerationStructureSize, 
				VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR, 
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
				VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT);
		_bottomScratchBuffer = std::make_shared<atta::vk::Buffer>(
				_device, total.buildScratchSize, 
				VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR, 
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
				VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT);

		// Generate the structures
		VkDeviceSize resultOffset = 0;
		VkDeviceSize scratchOffset = 0;

		for(size_t i = 0; i != _blas.size(); i++)
		{
			_blas[i]->generate(commandBuffer, _bottomBuffer, resultOffset, _bottomScratchBuffer, scratchOffset);
			resultOffset += _blas[i]->getBuildSizes().accelerationStructureSize;
			scratchOffset += _blas[i]->getBuildSizes().buildScratchSize;
		}
		// TODO after generating the BLAS it is possible to reduce the memory usage using a more fitted buffer
	}

	void RayTracing::createTopLevelStructures(VkCommandBuffer commandBuffer)
	{
		// Top level acceleration structure
		std::vector<VkAccelerationStructureInstanceKHR> instances;

		// Hit group 0: triangles
		// Hit group 1: procedurals
		for(std::shared_ptr<Object> object : _scene->getObjects())
		{
			std::shared_ptr<Model> model = object->getModel();
			if(model == nullptr)
				continue;

			mat4 transformation = atta::transpose(mat4(object->getModelMat()));

			//_blas[model->getModelIndex()]->getDevice();
			//std::cout << "INDEX: " << model->getMeshIndex() << std::endl;
			//std::cout << "Size: " << _blas.size() << std::endl;
			instances.push_back(TopLevelAccelerationStructure::createInstance(
				_blas[model->getMeshIndex()], transformation, model->getMeshIndex(), 0/*procedural?*/));
		}

		size_t size = instances.size()*sizeof(instances[0]);
		_instancesBuffer = std::make_shared<atta::vk::Buffer>(_device, size, 
				VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT, 
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
				VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT);
		// Copy data to instances buffer
		std::shared_ptr<atta::vk::StagingBuffer> stagingBuffer = std::make_shared<atta::vk::StagingBuffer>(_device, instances.data(), size);
		_instancesBuffer->copyFrom(_commandPool, stagingBuffer->handle(), size);
		stagingBuffer.reset();
		
		AccelerationStructure::memoryBarrier(commandBuffer);

		std::shared_ptr<TopLevelAccelerationStructure> tlas = std::make_shared<TopLevelAccelerationStructure>(_deviceProcedures, _rayTracingProperties, _instancesBuffer->getDeviceAddress(), instances.size());
		_tlas.push_back(tlas);

		// Calculate total memory size to allocate
		VkAccelerationStructureBuildSizesInfoKHR total = getTotalRequirements(_tlas);

		_topBuffer = std::make_shared<atta::vk::Buffer>(_device, total.accelerationStructureSize, 
				VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR, 
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		_topScratchBuffer = std::make_shared<atta::vk::Buffer>(_device, total.buildScratchSize, 
				VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT);

		// Generate the structures
		_tlas[0]->generate(commandBuffer, _topBuffer, 0, _topScratchBuffer, 0);
	}

	void RayTracing::recreateTLAS()
	{
		// Called every time some object changes
		//Log::info("rt::vk::RayTracing", "Recreating top level acceleration structures...");
		LocalEvaluator eval;

		// Delete TLAS objects
		_tlas.clear();
		_instancesBuffer.reset();
		_topBuffer.reset();
		_topScratchBuffer.reset();

		// Delete accumulation image
		_accumulationImage.reset();
		_accumulationImageView.reset();

		// Delete pipeline objects
		_rayTracingPipeline.reset();
		_shaderBindingTable.reset();

		VkCommandBuffer commandBuffer = _commandPool->beginSingleTimeCommands();
		{
			createTopLevelStructures(commandBuffer);
		}
		_commandPool->endSingleTimeCommands(commandBuffer);

		createAccumulationImage();
		createPipeline();

		eval.stop();
		//Log::info("rt::vk::RayTracing", "Finished: [w]$0ms", eval.getMs());
	}

	void RayTracing::updateCameraMatrix(mat4 viewMatrix)
	{
		vkDeviceWaitIdle(_device->handle());
		recreateTLAS();
		rt::vk::UniformBufferObject ubo = _uniformBuffer->getValue();
		ubo.viewMat = viewMatrix;
		ubo.viewMatInverse = ubo.viewMat;
		//Log::debug("RayTracing", "View: $0", (inverse(ubo.viewMat)).toString());
		ubo.totalNumberOfSamples = 0;
		_uniformBuffer->setValue(ubo);
	}
}
