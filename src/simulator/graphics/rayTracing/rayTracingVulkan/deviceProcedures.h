//--------------------------------------------------
// Atta Ray Tracing Vulkan
// deviceProcedures.h
// Date: 2020-07-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RT_VK_DEVICE_PROCEDURES_H
#define ATTA_RT_VK_DEVICE_PROCEDURES_H
#include <functional>
#include "simulator/graphics/vulkan/device.h"

namespace atta::rt::vk
{
	class DeviceProcedures
	{
		public:
			DeviceProcedures(std::shared_ptr<atta::vk::Device> device);
			~DeviceProcedures();

			std::shared_ptr<atta::vk::Device> getDevice() const { return _device; }

			const std::function<VkResult(
				VkDevice device,
				const VkAccelerationStructureCreateInfoKHR* pCreateInfo,
				const VkAllocationCallbacks* pAllocator,
				VkAccelerationStructureKHR* pAccelerationStructure)>
			vkCreateAccelerationStructureKHR;

			const std::function<void(
				VkDevice device,
				VkAccelerationStructureKHR accelerationStructure,
				const VkAllocationCallbacks* pAllocator)>
			vkDestroyAccelerationStructureKHR;

			const std::function<void(
				VkDevice device,
				VkAccelerationStructureBuildTypeKHR buildType,
				const VkAccelerationStructureBuildGeometryInfoKHR* pBuildInfo,
				const uint32_t* pMaxPrimitiveCounts,
				VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo)>
			vkGetAccelerationStructureBuildSizesKHR;

			const std::function<void(
				VkCommandBuffer commandBuffer,
				uint32_t infoCount,
				const VkAccelerationStructureBuildGeometryInfoKHR* pInfos,
				const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos)>
			vkCmdBuildAccelerationStructuresKHR;

			const std::function<void(
				VkCommandBuffer commandBuffer,
				const VkCopyAccelerationStructureInfoKHR* pInfo)>
			vkCmdCopyAccelerationStructureKHR;

			const std::function<void(
				VkCommandBuffer commandBuffer,
				const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable,
				const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable,
				const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable,
				const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable,
				uint32_t width,
				uint32_t height,
				uint32_t depth)>
			vkCmdTraceRaysKHR;

			const std::function<VkResult(
				VkDevice device,
				VkDeferredOperationKHR deferredOperation,
				VkPipelineCache pipelineCache,
				uint32_t createInfoCount,
				const VkRayTracingPipelineCreateInfoKHR* pCreateInfos,
				const VkAllocationCallbacks* pAllocator,
				VkPipeline* pPipelines)>
			vkCreateRayTracingPipelinesKHR;

			const std::function<VkResult(
				VkDevice device,
				VkPipeline pipeline,
				uint32_t firstGroup,
				uint32_t groupCount,
				size_t dataSize,
				void* pData)>
			vkGetRayTracingShaderGroupHandlesKHR;

			const std::function<VkDeviceAddress(
				VkDevice device,
				const VkAccelerationStructureDeviceAddressInfoKHR* pInfo)>
			vkGetAccelerationStructureDeviceAddressKHR;

			const std::function<void(
				VkCommandBuffer commandBuffer,
				uint32_t accelerationStructureCount,
				const VkAccelerationStructureKHR* pAccelerationStructures,
				VkQueryType queryType,
				VkQueryPool queryPool,
				uint32_t firstQuery)>
			vkCmdWriteAccelerationStructuresPropertiesKHR;

		private:
			std::shared_ptr<atta::vk::Device> _device;
	};
}

#endif// ATTA_RT_VK_DEVICE_PROCEDURES_H
