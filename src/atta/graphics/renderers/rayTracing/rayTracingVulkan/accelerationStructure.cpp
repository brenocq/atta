//--------------------------------------------------
// Atta Ray Tracing Vulkan
// acceletationStructure.cpp
// Date: 2020-07-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/renderers/rayTracing/rayTracingVulkan/accelerationStructure.h>
#include <atta/helpers/log.h>

namespace atta::rt::vk
{
	uint64_t roundUpAcc(uint64_t size, uint64_t granularity)
	{
		const auto divUp = (size + granularity - 1) / granularity;
		return divUp * granularity;
	}

	AccelerationStructure::AccelerationStructure(std::shared_ptr<DeviceProcedures> deviceProcedures, std::shared_ptr<RayTracingProperties> rayTracingProperties):
		_deviceProcedures(deviceProcedures),
		_flags(VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR),
		_device(deviceProcedures->getDevice()),
		_rayTracingProperties(rayTracingProperties)
	{
	}

	AccelerationStructure::~AccelerationStructure()
	{
		if (_accelerationStructure != nullptr)
		{
			_deviceProcedures->vkDestroyAccelerationStructureKHR(_device->handle(), _accelerationStructure, nullptr);
			_accelerationStructure = nullptr;
		}
	}

	VkAccelerationStructureBuildSizesInfoKHR AccelerationStructure::getBuildSizes(const uint32_t* pMaxPrimitiveCounts) const
	{
		// Query both the size of the finished acceleration structure and the amount of scratch memory needed
		VkAccelerationStructureBuildSizesInfoKHR sizeInfo = {};
		sizeInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;

		_deviceProcedures->vkGetAccelerationStructureBuildSizesKHR(
			_device->handle(),
			VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
			&_buildGeometryInfo,
			pMaxPrimitiveCounts,
			&sizeInfo);

		// AccelerationStructure offset needs to be 256 bytes aligned (official Vulkan specs)
		const uint64_t accelerationStructureAlignment = 256;
		const uint64_t scratchAlignment = _rayTracingProperties->minAccelerationStructureScratchOffsetAlignment();

		sizeInfo.accelerationStructureSize = roundUpAcc(sizeInfo.accelerationStructureSize, accelerationStructureAlignment);
		sizeInfo.buildScratchSize = roundUpAcc(sizeInfo.buildScratchSize, scratchAlignment);

		return sizeInfo;
	}

	void AccelerationStructure::createAccelerationStructure(std::shared_ptr<atta::vk::Buffer> resultBuffer, const VkDeviceSize resultOffset)
	{
		VkAccelerationStructureCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
		createInfo.pNext = nullptr;
		createInfo.type = _buildGeometryInfo.type;
		createInfo.size = getBuildSizes().accelerationStructureSize;
		createInfo.buffer = resultBuffer->handle();
		createInfo.offset = resultOffset;

		if(_deviceProcedures->vkCreateAccelerationStructureKHR(_device->handle(), &createInfo, nullptr, &_accelerationStructure) != VK_SUCCESS)
		{
			Log::error("rt::vk::AccelerationStructure", "Failed to create acceleration structure!");
		}
	}

	void AccelerationStructure::memoryBarrier(VkCommandBuffer commandBuffer)
	{
		// Wait for the builder to complete by setting a barrier on the resulting buffer. This is
		// particularly important as the construction of the top-level hierarchy may be called right
		// afterwards, before executing the command list.
		VkMemoryBarrier memoryBarrier = {};
		memoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
		memoryBarrier.pNext = nullptr;
		memoryBarrier.srcAccessMask = VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR | VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR;
		memoryBarrier.dstAccessMask = VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR | VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR;

		vkCmdPipelineBarrier(
			commandBuffer,
			VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_NV,
			VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_NV,
			0, 1, &memoryBarrier, 0, nullptr, 0, nullptr);
	}
}
