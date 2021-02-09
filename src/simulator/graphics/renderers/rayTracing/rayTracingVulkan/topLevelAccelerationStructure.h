//--------------------------------------------------
// Atta Ray Tracing Vulkan
// topLevelAcceletationStructure.h
// Date: 2020-07-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RT_VK_TOP_LEVEL_ACCELERATION_STRUCTURE_H
#define ATTA_RT_VK_TOP_LEVEL_ACCELERATION_STRUCTURE_H

#include "simulator/math/math.h"
#include "simulator/graphics/vulkan/buffer.h"
#include "accelerationStructure.h"
#include "bottomLevelAccelerationStructure.h"

namespace atta::rt::vk
{
	class TopLevelAccelerationStructure final : public AccelerationStructure
	{
		public:
			TopLevelAccelerationStructure(std::shared_ptr<DeviceProcedures> deviceProcedures, 
					std::shared_ptr<RayTracingProperties> rayTracingProperties, 
					VkDeviceAddress instanceAddress, 
					uint32_t instancesCount);
			~TopLevelAccelerationStructure();

			void generate(
				VkCommandBuffer commandBuffer,
				std::shared_ptr<atta::vk::Buffer> resultBuffer,
				VkDeviceSize resultOffset,
				std::shared_ptr<atta::vk::Buffer> scratchBuffer,
				VkDeviceSize scratchOffset);

			static VkAccelerationStructureInstanceKHR createInstance(
				std::shared_ptr<BottomLevelAccelerationStructure> bottomLevelAs,
				const mat4& transform,
				uint32_t instanceId,
				uint32_t hitGroupId);

		private:
			uint32_t _instancesCount;
			VkAccelerationStructureGeometryInstancesDataKHR _instancesVk;
			VkAccelerationStructureGeometryKHR _topASGeometry;
	};
}

#endif// ATTA_RT_VK_TOP_LEVEL_ACCELERATION_STRUCTURE_H
