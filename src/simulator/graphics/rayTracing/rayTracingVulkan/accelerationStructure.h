//--------------------------------------------------
// Atta Ray Tracing Vulkan
// acceletationStructure.h
// Date: 2020-07-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RT_VK_ACCELERATION_STRUCTURE_H
#define ATTA_RT_VK_ACCELERATION_STRUCTURE_H

#include "simulator/graphics/vulkan/device.h"
#include "simulator/graphics/vulkan/buffer.h"
#include "deviceProcedures.h"
#include "rayTracingProperties.h"

namespace atta::rt::vk
{

	class AccelerationStructure
	{
		public:
			virtual ~AccelerationStructure();

			std::shared_ptr<atta::vk::Device> getDevice() const { return _device; }
			std::shared_ptr<DeviceProcedures> getDeviceProcedures() const { return _deviceProcedures; }
			const VkAccelerationStructureBuildSizesInfoKHR getBuildSizes() const { return _buildSizesInfo; }
			VkAccelerationStructureKHR handle() const { return _accelerationStructure; }

			static void memoryBarrier(VkCommandBuffer commandBuffer);
			
		protected:
			AccelerationStructure(std::shared_ptr<DeviceProcedures> deviceProcedures, std::shared_ptr<RayTracingProperties> rayTracingProperties);

			VkAccelerationStructureBuildSizesInfoKHR getBuildSizes(const uint32_t* pMaxPrimitiveCounts) const;
			void createAccelerationStructure(std::shared_ptr<atta::vk::Buffer> resultBuffer, VkDeviceSize resultOffset);

			std::shared_ptr<DeviceProcedures> _deviceProcedures;
			const VkBuildAccelerationStructureFlagsKHR _flags;
			VkAccelerationStructureBuildGeometryInfoKHR _buildGeometryInfo;
			VkAccelerationStructureBuildSizesInfoKHR _buildSizesInfo;

		private:
			std::shared_ptr<atta::vk::Device> _device;
			std::shared_ptr<RayTracingProperties> _rayTracingProperties;
			VkAccelerationStructureKHR _accelerationStructure;
	};

	template <class TAccelerationStructure>
	VkAccelerationStructureBuildSizesInfoKHR getTotalRequirements(const std::vector<std::shared_ptr<TAccelerationStructure>>& accelerationStructures)
	{
		VkAccelerationStructureBuildSizesInfoKHR total;

		for (const auto& accelerationStructure : accelerationStructures)
		{
			total.accelerationStructureSize += accelerationStructure->getBuildSizes().accelerationStructureSize;
			total.buildScratchSize += accelerationStructure->getBuildSizes().buildScratchSize;
			total.updateScratchSize += accelerationStructure->getBuildSizes().updateScratchSize;
		}

		return total;
	}
}

#endif// ATTA_RT_VK_ACCELERATION_STRUCTURE_H
