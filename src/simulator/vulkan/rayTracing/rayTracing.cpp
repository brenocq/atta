//--------------------------------------------------
// Robot Simulator
// rayTracing.cpp
// Date: 15/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "rayTracing.h"
#include <chrono>
#include "../vertex.h"

RayTracing::RayTracing(Device* device, SwapChain* swapChain, CommandPool* commandPool, std::vector<UniformBuffer*> uniformBuffers, Scene* scene)
{
	_device = device;
	_swapChain = swapChain;
	_commandPool = commandPool;
	_uniformBuffers.assign(uniformBuffers.begin(), uniformBuffers.end());
	_scene = scene;
	_deviceProcedures = new DeviceProcedures(_device);
	_rayTracingPipeline = nullptr;

	getRTProperties();
	createAccelerationStructures();
	createSwapChain();
}

RayTracing::~RayTracing()
{
	if(_rayTracingPipeline!=nullptr)
	{
		delete _rayTracingPipeline;
		_rayTracingPipeline = nullptr;
	}

	if(_accumulationImage!=nullptr)
	{
		delete _accumulationImage;
		_accumulationImage = nullptr;
	}
	if(_accumulationImageView!=nullptr)
	{
		delete _accumulationImageView;
		_accumulationImageView = nullptr;
	}

	if(_outputImage!=nullptr)
	{
		delete _outputImage;
		_outputImage = nullptr;
	}
	if(_outputImageView!=nullptr)
	{
		delete _outputImageView;
		_outputImageView = nullptr;
	}
	
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

void RayTracing::createSwapChain()
{
	createOutputImage();
	_rayTracingPipeline = new RayTracingPipeline(_device, _deviceProcedures, _swapChain, _tlas[0], _accumulationImageView, _outputImageView, _uniformBuffers, _scene);
}

void RayTracing::deleteSwapChain()
{

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

void RayTracing::createBottomLevelStructures(VkCommandBuffer commandBuffer)
{
	// Bottom level acceleration structure
	// Triangles via vertex buffers. Procedurals via AABBs.
	uint32_t vertexOffset = 0;
	uint32_t indexOffset = 0;
	//uint32_t aabbOffset = 0;

	std::vector<AccelerationStructure::MemoryRequirements> requirements;

	// Create blas object with memory requirements for each model
	for(Model* model : _scene->getModels())
	{
		const auto vertexCount = static_cast<uint32_t>(model->getVertices().size());
		const auto indexCount = static_cast<uint32_t>(model->getIndices().size());
		const std::vector<VkGeometryNV> geometries =
		{
			//model.isProcedural()
			//	? BottomLevelAccelerationStructure::CreateGeometryAabb(scene, aabbOffset, 1, true)
			//	: BottomLevelAccelerationStructure::CreateGeometry(scene, vertexOffset, vertexCount, indexOffset, indexCount, true)
			BottomLevelAccelerationStructure::createGeometry(_scene, vertexOffset, vertexCount, indexOffset, indexCount, true)
		};

		_blas.emplace_back(_deviceProcedures, geometries, false);
		requirements.push_back(_blas.back().getMemoryRequirements());

		vertexOffset += vertexCount * sizeof(Vertex);
		indexOffset += indexCount * sizeof(uint32_t);
		//aabbOffset += sizeof(glm::vec3) * 2;
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

	for (size_t i = 0; i != _blas.size(); i++)
	{
		_blas[i].generate(commandBuffer, _bottomBuffer, resultOffset, _bottomScratchBuffer, scratchOffset, false);
		resultOffset += requirements[i].result.size;
		scratchOffset += requirements[i].build.size;
	}
}

void RayTracing::createTopLevelStructures(VkCommandBuffer commandBuffer)
{
	// Top level acceleration structure
	std::vector<VkGeometryInstance> geometryInstances;
	std::vector<AccelerationStructure::MemoryRequirements> requirements;

	// Hit group 0: triangles
	// Hit group 1: procedurals
	uint32_t instanceId = 0;

	for (Model* model : _scene->getModels())
	{
		geometryInstances.push_back(TopLevelAccelerationStructure::createGeometryInstance(
			_blas[instanceId], glm::mat4(1), instanceId, 0));
		instanceId++;
	}

	_tlas.emplace_back(_deviceProcedures, geometryInstances, false);
	requirements.push_back(_tlas.back().getMemoryRequirements());

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
	_tlas[0].generate(commandBuffer, _topBuffer, 0, _topScratchBuffer, 0, _instancesBuffer, 0, false);
}

void RayTracing::createOutputImage()
{
	const auto extent = _swapChain->getExtent();
	const auto format = _swapChain->getImageFormat();
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
