//--------------------------------------------------
// Robot Simulator
// rayTracing.cpp
// Date: 2020-07-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "rayTracing.h"
#include <chrono>
#include "../vertex.h"
#include "../imageMemoryBarrier.h"

RayTracing::RayTracing(
		std::shared_ptr<Device> device, 
		std::shared_ptr<SwapChain> swapChain, 
		std::shared_ptr<CommandPool> commandPool, 
		std::vector<UniformBuffer*> uniformBuffers, 
		Scene* scene):
	RayTracing(device, swapChain, swapChain->getExtent(), swapChain->getImageFormat(), commandPool, uniformBuffers, scene)
{
}

RayTracing::RayTracing(
		std::shared_ptr<Device> device, 
		VkExtent2D extent, VkFormat format, 
		std::shared_ptr<CommandPool> commandPool, 
		UniformBuffer* uniformBuffer, 
		Scene* scene):
	RayTracing(device, std::shared_ptr<SwapChain>(nullptr), extent, format, commandPool, std::vector<UniformBuffer*>{uniformBuffer}, scene)
{
}

RayTracing::RayTracing(
		std::shared_ptr<Device> device, 
		std::shared_ptr<SwapChain> swapChain, 
		VkExtent2D extent, VkFormat format, 
		std::shared_ptr<CommandPool> commandPool, 
		std::vector<UniformBuffer*> uniformBuffers, 
		Scene* scene):
	_device(device), _swapChain(swapChain), _commandPool(commandPool), _imageExtent(extent), _imageFormat(format)
{
	_uniformBuffers = uniformBuffers;
	_scene = scene;
	_deviceProcedures = new DeviceProcedures(_device);
	_rayTracingPipeline = nullptr;

	getRTProperties();
	createAccelerationStructures();
	createPipeline();
}

RayTracing::~RayTracing()
{
	deletePipeline();

	for(auto blas : _blas)
	{
		delete blas;
		blas = nullptr;
	}

	for(auto tlas : _tlas)
	{
		delete tlas;
		tlas = nullptr;
	}

	// Device Procedures
	if(_deviceProcedures!=nullptr)
	{
		delete _deviceProcedures;
		_deviceProcedures = nullptr;
	}

	// BLAS buffers
	if(_bottomBuffer!=nullptr)
	{
		delete _bottomBuffer;
		_bottomBuffer = nullptr;
	}

	if(_bottomScratchBuffer!=nullptr)
	{
		delete _bottomScratchBuffer;
		_bottomScratchBuffer = nullptr;
	}

	// TLAS buffers
	if(_topBuffer!=nullptr)
	{
		delete _topBuffer;
		_topBuffer = nullptr;
	}

	if(_topScratchBuffer!=nullptr)
	{
		delete _topScratchBuffer;
		_topScratchBuffer = nullptr;
	}

	if(_instancesBuffer!=nullptr)
	{
		delete _instancesBuffer;
		_instancesBuffer = nullptr;
	}
}

void RayTracing::createPipeline()
{
	createOutputImage();
	if(_swapChain!=nullptr)
		_rayTracingPipeline = new RayTracingPipeline(_device, _deviceProcedures, _swapChain->getImages().size(), 
				_tlas[0], _accumulationImageView, _outputImageView, _uniformBuffers, _scene);
	else
		_rayTracingPipeline = new RayTracingPipeline(_device, _deviceProcedures, 1, 
				_tlas[0], _accumulationImageView, _outputImageView, _uniformBuffers, _scene);

	const std::vector<ShaderBindingTable::Entry> rayGenPrograms = { {_rayTracingPipeline->getRayGenShaderIndex(), {}} };
	const std::vector<ShaderBindingTable::Entry> missPrograms = { {_rayTracingPipeline->getMissShaderIndex(), {}} };
	const std::vector<ShaderBindingTable::Entry> hitGroups = { {_rayTracingPipeline->getTriangleHitGroupIndex(), {}}, {_rayTracingPipeline->getProceduralHitGroupIndex(), {}}};

	_shaderBindingTable = new ShaderBindingTable(_device, _deviceProcedures, _rayTracingPipeline, _props, rayGenPrograms, missPrograms, hitGroups);
}

void RayTracing::deletePipeline()
{
	// Images
	if(_accumulationImageView!=nullptr)
	{
		delete _accumulationImageView;
		_accumulationImageView = nullptr;
	}
	if(_accumulationImage!=nullptr)
	{
		delete _accumulationImage;
		_accumulationImage = nullptr;
	}

	if(_outputImageView!=nullptr)
	{
		delete _outputImageView;
		_outputImageView = nullptr;
	}
	if(_outputImage!=nullptr)
	{
		delete _outputImage;
		_outputImage = nullptr;
	}
	
	// Shader binding table
	if(_shaderBindingTable!=nullptr)
	{
		delete _shaderBindingTable;
		_shaderBindingTable = nullptr;
	}

	// Ray tracing pipeline
	if(_rayTracingPipeline!=nullptr)
	{
		delete _rayTracingPipeline;
		_rayTracingPipeline = nullptr;
	}
}

void RayTracing::getRTProperties()
{
	_props.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PROPERTIES_NV;

	VkPhysicalDeviceProperties2 props = {};
	props.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
	props.pNext = &_props;
	vkGetPhysicalDeviceProperties2(_device->getPhysicalDevice()->handle(), &props);
}

void RayTracing::createAccelerationStructures()
{
	std::cout << std::endl << BOLDGREEN << "[RayTracing]" << GREEN << " Building acceleration structures... ";
	const auto timer = std::chrono::high_resolution_clock::now();

	VkCommandBuffer commandBuffer = _commandPool->beginSingleTimeCommands();
	{
		createBottomLevelStructures(commandBuffer);
		AccelerationStructure::memoryBarrier(commandBuffer);
		createTopLevelStructures(commandBuffer);
	}
	_commandPool->endSingleTimeCommands(commandBuffer);

	const auto elapsed = std::chrono::duration<float, std::chrono::milliseconds::period>(std::chrono::high_resolution_clock::now() - timer).count();
	std::cout << WHITE << elapsed << "ms" << RESET << std::endl;
}

void RayTracing::recreateTopLevelStructures()
{
	std::cout << std::endl << BOLDGREEN << "[RayTracing]" << GREEN << " Recreating top level acceleration structures... ";
	const auto timer = std::chrono::high_resolution_clock::now();

	deletePipeline();
	// Delete tlas
	for(auto tlas : _tlas)
	{
		delete tlas;
		tlas = nullptr;
	}
	_tlas.clear();

	// Delete tlas buffers
	if(_topBuffer!=nullptr)
	{
		delete _topBuffer;
		_topBuffer = nullptr;
	}

	if(_topScratchBuffer!=nullptr)
	{
		delete _topScratchBuffer;
		_topScratchBuffer = nullptr;
	}

	if(_instancesBuffer!=nullptr)
	{
		delete _instancesBuffer;
		_instancesBuffer = nullptr;
	}

	// Update instance buffer
	_scene->updateRayTracingBuffers();
	// Create tlas
	VkCommandBuffer commandBuffer = _commandPool->beginSingleTimeCommands();
	{
		createTopLevelStructures(commandBuffer);
	}
	_commandPool->endSingleTimeCommands(commandBuffer);

	const auto elapsed = std::chrono::duration<float, std::chrono::milliseconds::period>(std::chrono::high_resolution_clock::now() - timer).count();
	std::cout << WHITE << elapsed << "ms" << RESET << std::endl;

	createPipeline();
}

void RayTracing::createOutputImage()
{
	const VkExtent2D extent = _imageExtent;
	const VkFormat format = _imageFormat;
	const auto tiling = VK_IMAGE_TILING_OPTIMAL;

	_accumulationImage = new Image(_device, 
			extent.width, extent.height, 
			VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_STORAGE_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	_accumulationImageView = new ImageView(_device, _accumulationImage->handle(), VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_ASPECT_COLOR_BIT);

	_outputImage = new Image(_device, 
			extent.width, extent.height, 
			format, tiling, VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	_outputImageView = new ImageView(_device, _outputImage->handle(), format, VK_IMAGE_ASPECT_COLOR_BIT);
}

void RayTracing::render(VkCommandBuffer commandBuffer, const uint32_t imageIndex, bool split)
{
	const VkExtent2D extent = _imageExtent;

	VkDescriptorSet descriptorSets[] = { _rayTracingPipeline->getDescriptorSet(imageIndex) };

	VkImageSubresourceRange subresourceRange;
	subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	subresourceRange.baseMipLevel = 0;
	subresourceRange.levelCount = 1;
	subresourceRange.baseArrayLayer = 0;
	subresourceRange.layerCount = 1;

	ImageMemoryBarrier::insert(commandBuffer, _accumulationImage->handle(), subresourceRange, 0, 
		VK_ACCESS_SHADER_WRITE_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);

	ImageMemoryBarrier::insert(commandBuffer, _outputImage->handle(), subresourceRange, 0, 
		VK_ACCESS_SHADER_WRITE_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_RAY_TRACING_NV, _rayTracingPipeline->handle());
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_RAY_TRACING_NV, _rayTracingPipeline->getPipelineLayout()->handle(), 0, 1, descriptorSets, 0, nullptr);

	// Ray tracing to output/accumulation image
	_deviceProcedures->vkCmdTraceRaysNV(commandBuffer,
		_shaderBindingTable->getBuffer()->handle(), _shaderBindingTable->getRayGenOffset(),
		_shaderBindingTable->getBuffer()->handle(), _shaderBindingTable->getMissOffset(), _shaderBindingTable->getMissEntrySize(),
		_shaderBindingTable->getBuffer()->handle(), _shaderBindingTable->getHitGroupOffset(), _shaderBindingTable->getHitGroupEntrySize(),
		nullptr, 0, 0,
		extent.width, extent.height, 1);

	// Transfer the image to swapChain if its not rendering offline 
	if(_swapChain!=nullptr)
	{
		ImageMemoryBarrier::insert(commandBuffer, _outputImage->handle(), subresourceRange, 
			VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

		ImageMemoryBarrier::insert(commandBuffer, _swapChain->getImages()[imageIndex], subresourceRange, 
			0, VK_ACCESS_TRANSFER_WRITE_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		// Copy all image or only half (splits the screen in half)
		VkImageCopy copyRegion;
		copyRegion.srcSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
		copyRegion.srcOffset = { split?(int32_t)extent.width/2:0, 0, 0 };
		copyRegion.dstSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
		copyRegion.dstOffset = { split?(int32_t)extent.width/2:0, 0, 0 };
		copyRegion.extent = { split?(int32_t)extent.width/2:extent.width, extent.height, 1 };

		vkCmdCopyImage(commandBuffer,
			_outputImage->handle(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			_swapChain->getImages()[imageIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &copyRegion);

		// Changed VK_IMAGE_LAYOUT_PRESENT_SRC_KHR to VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL (imgui)
		ImageMemoryBarrier::insert(commandBuffer, _swapChain->getImages()[imageIndex], subresourceRange, VK_ACCESS_TRANSFER_WRITE_BIT,
			0, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	}
}

void RayTracing::createBottomLevelStructures(VkCommandBuffer commandBuffer)
{
	// Bottom level acceleration structure
	// Triangles via vertex buffers. Procedurals via AABBs.
	uint32_t aabbOffset = 0;

	std::vector<AccelerationStructure::MemoryRequirements> requirements;
	std::vector<int> indexesGenerated = {};

	// Create blas object with memory requirements for each model
	for(Model* model : _scene->getModels())
	{
		int modelIndex = model->getModelIndex();
		// Generate only one per model
		bool found = false;
		for(auto index : indexesGenerated)
			if(index == modelIndex)
			{
				found = true;
				break;
			}
		if(found)
			continue;
		indexesGenerated.push_back(modelIndex);

		const uint32_t vertexCount = model->getVerticesSize();
		const uint32_t indexCount = model->getIndicesSize();
		const uint32_t vertexOffset = model->getVertexOffset()*sizeof(Vertex);
		const uint32_t indexOffset = model->getIndexOffset()*sizeof(uint32_t);
		const std::vector<VkGeometryNV> geometries =
		{
			model->getProcedural()
				? BottomLevelAccelerationStructure::createGeometryAabb(_scene, aabbOffset, 1, true)
				: BottomLevelAccelerationStructure::createGeometry(_scene, vertexOffset, vertexCount, indexOffset, indexCount, true)
		};

		BottomLevelAccelerationStructure* blas = new BottomLevelAccelerationStructure(_deviceProcedures, geometries, false);
		_blas.push_back(blas);
		requirements.push_back(_blas.back()->getMemoryRequirements());

		aabbOffset += sizeof(glm::vec3) * 2;
	}

	// Allocate the structure memory

	// Calculate total memory size to allocate
	AccelerationStructure::MemoryRequirements total{};
	for(const auto req : requirements)
	{
		total.result.size += req.result.size;
		total.build.size += req.build.size;
		total.update.size += req.update.size;
	}

	_bottomBuffer = new Buffer(_device, total.result.size, VK_BUFFER_USAGE_RAY_TRACING_BIT_NV, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	_bottomScratchBuffer = new Buffer(_device, total.build.size, VK_BUFFER_USAGE_RAY_TRACING_BIT_NV, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	// Generate the structures
	VkDeviceSize resultOffset = 0;
	VkDeviceSize scratchOffset = 0;

	for(size_t i = 0; i != _blas.size(); i++)
	{
		_blas[i]->generate(commandBuffer, _bottomBuffer, resultOffset, _bottomScratchBuffer, scratchOffset, false);
		resultOffset += requirements[i].result.size;
		scratchOffset += requirements[i].build.size;
	}
	// TODO after generating the BLAS it is possible to reduce the memory usage using a more fitted buffer
}

void RayTracing::createTopLevelStructures(VkCommandBuffer commandBuffer)
{
	// Top level acceleration structure
	std::vector<VkGeometryInstance> geometryInstances;
	std::vector<AccelerationStructure::MemoryRequirements> requirements;

	// Hit group 0: triangles
	// Hit group 1: procedurals
	for(Object* object : _scene->getObjects())
	{
		Model* model = object->getModel();
		if(model == nullptr)
			continue;

		// glm::mat4 to expected by nvidia
		glm::mat4 transformation = glm::transpose(glm::mat4(object->getModelMat()));

		//_blas[model->getModelIndex()]->getDevice();
		//std::cout << "INDEX: " << model->getModelIndex() << std::endl;
		//std::cout << "Size: " << _blas.size() << std::endl;
		geometryInstances.push_back(TopLevelAccelerationStructure::createGeometryInstance(
			_blas[model->getModelIndex()], transformation, model->getModelIndex(), model->getProcedural()?1:0));
	}

	TopLevelAccelerationStructure* tlas = new TopLevelAccelerationStructure(_deviceProcedures, geometryInstances, false);
	_tlas.push_back(tlas);
	requirements.push_back(_tlas.back()->getMemoryRequirements());

	// Calculate total memory size to allocate
	AccelerationStructure::MemoryRequirements total{};
	for(const auto req : requirements)
	{
		total.result.size += req.result.size;
		total.build.size += req.build.size;
		total.update.size += req.update.size;
	}

	_topBuffer = new Buffer(_device, total.result.size, VK_BUFFER_USAGE_RAY_TRACING_BIT_NV, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	_topScratchBuffer = new Buffer(_device, total.build.size, VK_BUFFER_USAGE_RAY_TRACING_BIT_NV, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	const size_t instancesBufferSize = sizeof(VkGeometryInstance) * geometryInstances.size();
	_instancesBuffer = new Buffer(_device, instancesBufferSize, VK_BUFFER_USAGE_RAY_TRACING_BIT_NV, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// Generate the structures.
	_tlas[0]->generate(commandBuffer, _topBuffer, 0, _topScratchBuffer, 0, _instancesBuffer, 0, false);
}
