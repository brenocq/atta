//--------------------------------------------------
// Robot Simulator
// bottomLevelAcceletationStructure.cpp
// Date: 2020-07-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "bottomLevelAccelerationStructure.h"
#include "../vertex.h"

VkAccelerationStructureCreateInfoNV getCreateInfo(const std::vector<VkGeometryNV>& geometries, const bool allowUpdate)
{
	const auto flags = allowUpdate 
		? VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_NV 
		: VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_NV;

	VkAccelerationStructureCreateInfoNV structureInfo = {};
	structureInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_NV;
	structureInfo.pNext = nullptr;
	structureInfo.compactedSize = 0;
	structureInfo.info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_INFO_NV;
	structureInfo.info.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_NV;
	structureInfo.info.flags = flags;
	structureInfo.info.instanceCount = 0; // The bottom-level AS can only contain explicit geometry, and no instances
	structureInfo.info.geometryCount = static_cast<uint32_t>(geometries.size());
	structureInfo.info.pGeometries = geometries.data();

	return structureInfo;
}

BottomLevelAccelerationStructure::BottomLevelAccelerationStructure(
	DeviceProcedures* deviceProcedures, 
	const std::vector<VkGeometryNV>& geometries, 
	const bool allowUpdate):
	AccelerationStructure(deviceProcedures, getCreateInfo(geometries, allowUpdate)),
	_geometries(geometries)
{

}

BottomLevelAccelerationStructure::~BottomLevelAccelerationStructure()
{

}

void BottomLevelAccelerationStructure::generate(
	VkCommandBuffer commandBuffer, 
	Buffer* resultBuffer,
	const VkDeviceSize resultOffset,
	Buffer* scratchBuffer,
	const VkDeviceSize scratchOffset,
	const bool updateOnly) const
{
	if(updateOnly && !_allowUpdate)
	{
		std::cerr << BOLDYELLOW << "[BottomLevelAccelerationStructure]" << YELLOW << " Cannot update readonly structure!" << RESET << std::endl;
		return;
	}

	const VkAccelerationStructureNV previousStructure = updateOnly ? handle() : nullptr;

	// Bind the acceleration structure descriptor to the actual memory that will contain it
	VkBindAccelerationStructureMemoryInfoNV bindInfo = {};	
	bindInfo.sType = VK_STRUCTURE_TYPE_BIND_ACCELERATION_STRUCTURE_MEMORY_INFO_NV;
	bindInfo.pNext = nullptr;
	bindInfo.accelerationStructure = handle();
	bindInfo.memory = resultBuffer->getMemory();
	bindInfo.memoryOffset = resultOffset;
	bindInfo.deviceIndexCount = 0;
	bindInfo.pDeviceIndices = nullptr;

	if(_deviceProcedures->vkBindAccelerationStructureMemoryNV(getDevice()->handle(), 1, &bindInfo) != VK_SUCCESS)
	{
		std::cerr << BOLDRED << "[BottomLevelAccelerationStructure]" << RED << " Failed to bind acceleration structure!" << RESET << std::endl;
		exit(1);
	}

	// Build the actual bottom-level acceleration structure
	const auto flags = _allowUpdate
		? VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_NV 
		: VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_NV;
	
	VkAccelerationStructureInfoNV buildInfo = {};
	buildInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_INFO_NV;
	buildInfo.pNext = nullptr;
	buildInfo.flags = flags;
	buildInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_NV;
	buildInfo.instanceCount = 0;
	buildInfo.geometryCount = static_cast<uint32_t>(_geometries.size());
	buildInfo.pGeometries = _geometries.data();

	_deviceProcedures->vkCmdBuildAccelerationStructureNV(
		commandBuffer, &buildInfo, nullptr, 0, updateOnly, handle(), previousStructure, scratchBuffer->handle(), scratchOffset);
}

VkGeometryNV BottomLevelAccelerationStructure::createGeometry(
	Scene* scene,
	const uint32_t vertexOffset, const uint32_t vertexCount,
	const uint32_t indexOffset, const uint32_t indexCount,
	const bool isOpaque)
{
	VkGeometryNV geometry = {};
	geometry.sType = VK_STRUCTURE_TYPE_GEOMETRY_NV;
	geometry.pNext = nullptr;
	geometry.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_NV;
	geometry.geometry.triangles.sType = VK_STRUCTURE_TYPE_GEOMETRY_TRIANGLES_NV;
	geometry.geometry.triangles.pNext = nullptr;
	geometry.geometry.triangles.vertexData = scene->getVertexBuffer()->handle();
	geometry.geometry.triangles.vertexOffset = vertexOffset;
	geometry.geometry.triangles.vertexCount = vertexCount;
	geometry.geometry.triangles.vertexStride = sizeof(Vertex);
	geometry.geometry.triangles.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT;
	geometry.geometry.triangles.indexData = scene->getIndexBuffer()->handle();
	geometry.geometry.triangles.indexOffset = indexOffset;
	geometry.geometry.triangles.indexCount = indexCount;
	geometry.geometry.triangles.indexType = VK_INDEX_TYPE_UINT32;
	geometry.geometry.triangles.transformData = nullptr;
	geometry.geometry.triangles.transformOffset = 0;
	geometry.geometry.aabbs.sType = VK_STRUCTURE_TYPE_GEOMETRY_AABB_NV;
	geometry.flags = isOpaque ? VK_GEOMETRY_OPAQUE_BIT_NV : 0;

	return geometry;
}

VkGeometryNV BottomLevelAccelerationStructure::createGeometryAabb(
	Scene* scene,
	const uint32_t aabbOffset,
	const uint32_t aabbCount,
	const bool isOpaque)
{
	VkGeometryNV geometry = {};
	geometry.sType = VK_STRUCTURE_TYPE_GEOMETRY_NV;
	geometry.pNext = nullptr;
	geometry.geometryType = VK_GEOMETRY_TYPE_AABBS_NV;
	geometry.geometry.triangles.sType = VK_STRUCTURE_TYPE_GEOMETRY_TRIANGLES_NV;
	geometry.geometry.aabbs.sType = VK_STRUCTURE_TYPE_GEOMETRY_AABB_NV;
	geometry.geometry.aabbs.pNext = nullptr;
	geometry.geometry.aabbs.aabbData = scene->getAabbBuffer()->handle();
	geometry.geometry.aabbs.numAABBs = aabbCount;
	geometry.geometry.aabbs.stride = sizeof(glm::vec3) * 2;
	geometry.geometry.aabbs.offset = aabbOffset;
	geometry.flags = isOpaque ? VK_GEOMETRY_OPAQUE_BIT_NV : 0;

	return geometry;
}
