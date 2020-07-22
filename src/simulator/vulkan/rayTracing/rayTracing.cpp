//--------------------------------------------------
// Robot Simulator
// rayTracing.cpp
// Date: 15/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "rayTracing.h"
#include <chrono>
#include "../vertex.h"

RayTracing::RayTracing(Device* device, CommandPool* commandPool, Scene* scene)
{
	_device = device;
	_commandPool = commandPool;
	_scene = scene;
	_deviceProcedures = new DeviceProcedures(_device);

	getRTProperties();
	createAccelerationStructures();
}

RayTracing::~RayTracing()
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

	createBottomLevelStructures(commandBuffer);
		//AccelerationStructure::MemoryBarrier(commandBuffer);
		//CreateTopLevelStructures(commandBuffer);

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

	//std::vector<AccelerationStructure::MemoryRequirements> requirements;

	// Create blas object with memory requirements for each model
	//for(Model model : _scene.getModels())
	//{
	//	const auto vertexCount = static_cast<uint32_t>(model.getVertices().size();
	//	const auto indexCount = static_cast<uint32_t>(model.getIndices().size());
	//	const std::vector<VkGeometryNV> geometries =
	//	{
	//		//model.isProcedural()
	//		//	? BottomLevelAccelerationStructure::CreateGeometryAabb(scene, aabbOffset, 1, true)
	//		//	: BottomLevelAccelerationStructure::CreateGeometry(scene, vertexOffset, vertexCount, indexOffset, indexCount, true)
	//		BottomLevelAccelerationStructure::createGeometry(_scene, vertexOffset, vertexCount, indexOffset, indexCount, true)
	//	};

	//	_blas.emplace_back(*_deviceProcedures, geometries, false);
	//	requirements.push_back(_blas.back().getMemoryRequirements());

	//	vertexOffset += vertexCount * sizeof(Vertex);
	//	indexOffset += indexCount * sizeof(uint32_t);
	//	//aabbOffset += sizeof(glm::vec3) * 2;
	//}

	//// Allocate the structure memory.

	//// Calculate total memory size to allocate
	//AccelerationStructure::MemoryRequirements total{};
	//for(const auto req : requirements)
	//{
	//	total.result.size += req.result.size;
	//	total.build.size += req.build.size;
	//	total.update.size += req.update.size;
	//}

	//_bottomBuffer = new Buffer(_device, total.result.size, VK_BUFFER_USAGE_RAY_TRACING_BIT_NV, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
	//_bottomScratchBuffer = new Buffer(_device, total.build.size, VK_BUFFER_USAGE_RAY_TRACING_BIT_NV, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	//// Generate the structures.
	//VkDeviceSize resultOffset = 0;
	//VkDeviceSize scratchOffset = 0;

	//for (size_t i = 0; i != _blas.size(); i++)
	//{
	//	_blas[i].generate(commandBuffer, *_bottomBuffer, resultOffset, *_bottomScratchBuffer, scratchOffset, false);
	//	resultOffset += requirements[i].result.size;
	//	scratchOffset += requirements[i].build.size;
	//}
}
