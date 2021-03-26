//--------------------------------------------------
// Atta Ray Tracing Vulkan
// bottomLevelAcceletationStructure.h
// Date: 2020-07-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RT_VK_BOTTOM_LEVEL_ACCELERATION_STRUCTURE_H
#define ATTA_RT_VK_BOTTOM_LEVEL_ACCELERATION_STRUCTURE_H

#include <vector>
#include <atta/graphics/renderers/rayTracing/rayTracingVulkan/accelerationStructure.h>
#include <atta/graphics/renderers/rayTracing/rayTracingVulkan/bottomLevelGeometry.h>
#include <atta/graphics/vulkan/buffer.h>
#include <atta/graphics/vulkan/vulkanCore.h>

namespace atta::rt::vk
{
	class BottomLevelAccelerationStructure final : public AccelerationStructure
	{
		public:
			BottomLevelAccelerationStructure(
					std::shared_ptr<DeviceProcedures> deviceProcedures, 
					std::shared_ptr<RayTracingProperties> rayTracingProperties, 
					std::shared_ptr<BottomLevelGeometry> geometries);
			~BottomLevelAccelerationStructure();

			void generate(
				VkCommandBuffer commandBuffer,
				std::shared_ptr<atta::vk::Buffer> resultBuffer,
				VkDeviceSize resultOffset,
				std::shared_ptr<atta::vk::Buffer> scratchBuffer,
				VkDeviceSize scratchOffset);

		private:
			std::shared_ptr<BottomLevelGeometry> _geometries;
	};
}

#endif// ATTA_RT_VK_BOTTOM_LEVEL_ACCELERATION_STRUCTURE_H
