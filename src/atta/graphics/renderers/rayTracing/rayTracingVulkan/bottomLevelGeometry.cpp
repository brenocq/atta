//--------------------------------------------------
// Atta Ray Tracing Vulkan
// bottomLevelGeometry.cpp
// Date: 2021-02-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/renderers/rayTracing/rayTracingVulkan/bottomLevelGeometry.h>
#include <atta/graphics/core/vertex.h>

namespace atta::rt::vk
{
	void BottomLevelGeometry::addGeometry(
		std::shared_ptr<atta::vk::VulkanCore> vkCore,
		const uint32_t vertexOffset, const uint32_t vertexCount,
		const uint32_t indexOffset, const uint32_t indexCount,
		const bool isOpaque)
	{
		VkAccelerationStructureGeometryKHR geometry = {};
		geometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
		geometry.pNext = nullptr;
		geometry.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR;
		geometry.geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
		geometry.geometry.triangles.pNext = nullptr;
		geometry.geometry.triangles.vertexData.deviceAddress = vkCore->getVertexBuffer()->getDeviceAddress();
		geometry.geometry.triangles.vertexStride = sizeof(Vertex);
		geometry.geometry.triangles.maxVertex = vertexCount;
		geometry.geometry.triangles.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT;
		geometry.geometry.triangles.indexData.deviceAddress = vkCore->getIndexBuffer()->getDeviceAddress();
		geometry.geometry.triangles.indexType = VK_INDEX_TYPE_UINT32;
		geometry.geometry.triangles.transformData = {};
		geometry.flags = isOpaque ? VK_GEOMETRY_OPAQUE_BIT_KHR : 0;

		VkAccelerationStructureBuildRangeInfoKHR buildOffsetInfo = {};
		buildOffsetInfo.firstVertex = vertexOffset / sizeof(Vertex);
		buildOffsetInfo.primitiveOffset = indexOffset;
		buildOffsetInfo.primitiveCount = indexCount / 3;
		buildOffsetInfo.transformOffset = 0;

		_geometry.emplace_back(geometry);
		_buildOffsetInfo.emplace_back(buildOffsetInfo);
	}

	//void BottomLevelGeometry::AddGeometryAabb(
	//	std::shared_ptr<VulkanCore> vkCore,
	//	const uint32_t aabbOffset,
	//	const uint32_t aabbCount,
	//	const bool isOpaque)
	//{
	//	VkAccelerationStructureGeometryKHR geometry = {};
	//	geometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
	//	geometry.pNext = nullptr;
	//	geometry.geometryType = VK_GEOMETRY_TYPE_AABBS_KHR;
	//	geometry.geometry.aabbs.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_AABBS_DATA_KHR;
	//	geometry.geometry.aabbs.pNext = nullptr;
	//	geometry.geometry.aabbs.data.deviceAddress = vkCore->getAabbBuffer()->getDeviceAddress();
	//	geometry.geometry.aabbs.stride = sizeof(VkAabbPositionsKHR);
	//	geometry.flags = isOpaque ? VK_GEOMETRY_OPAQUE_BIT_KHR : 0;

	//	VkAccelerationStructureBuildRangeInfoKHR buildOffsetInfo = {};
	//	buildOffsetInfo.firstVertex = 0;
	//	buildOffsetInfo.primitiveOffset = aabbOffset;
	//	buildOffsetInfo.primitiveCount = aabbCount;
	//	buildOffsetInfo.transformOffset = 0;

	//	_geometry.emplace_back(geometry);
	//	_buildOffsetInfo.emplace_back(buildOffsetInfo);
	//}
}

