//--------------------------------------------------
// Atta Ray Tracing Vulkan
// bottomLevelGeometry.h
// Date: 2021-02-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RT_VK_BOTTOM_LEVEL_GEOMETRY_H
#define ATTA_RT_VK_BOTTOM_LEVEL_GEOMETRY_H

#include <vector>
#include <atta/graphics/vulkan/vulkanCore.h>

namespace atta::rt::vk
{
	class BottomLevelGeometry final
	{
		public:
			size_t size() const { return _geometry.size(); }
			const std::vector<VkAccelerationStructureGeometryKHR>& geometry() const { return _geometry; }
			const std::vector<VkAccelerationStructureBuildRangeInfoKHR>& buildOffsetInfo() const { return _buildOffsetInfo; }

			void addGeometry(
				std::shared_ptr<atta::vk::VulkanCore> vkCore,
				uint32_t vertexOffset,
				uint32_t vertexCount,
				uint32_t indexOffset,
				uint32_t indexCount,
				bool isOpaque);

			//void AddGeometryAabb(
			//	std::shared_ptr<VulkanCore> vkCore,
			//	uint32_t aabbOffset,
			//	uint32_t aabbCount,
			//	bool isOpaque);

		private:
			// The geometry to build, addresses of vertices and indices
			std::vector<VkAccelerationStructureGeometryKHR> _geometry;

			// The number of elements to build and offsets
			std::vector<VkAccelerationStructureBuildRangeInfoKHR> _buildOffsetInfo;
	};
}

#endif// ATTA_RT_VK_BOTTOM_LEVEL_GEOMETRY_H
