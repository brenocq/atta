//--------------------------------------------------
// Atta Ray Tracing Vulkan
// topLevelAcceletationStructure.cpp
// Date: 2020-07-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "topLevelAccelerationStructure.h"
#include <cstring>

namespace atta::rt::vk
{
	TopLevelAccelerationStructure::TopLevelAccelerationStructure(
		std::shared_ptr<DeviceProcedures> deviceProcedures,
		std::shared_ptr<RayTracingProperties> rayTracingProperties,
		const VkDeviceAddress instanceAddress,
		const uint32_t instancesCount) :
			AccelerationStructure(deviceProcedures, rayTracingProperties),
			_instancesCount(instancesCount)
	{
		// Create VkAccelerationStructureGeometryInstancesDataKHR. This wraps a device pointer to the above uploaded instances.
		_instancesVk = {};
		_instancesVk.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
		_instancesVk.arrayOfPointers = VK_FALSE;
		_instancesVk.data.deviceAddress = instanceAddress;

		// Put the above into a VkAccelerationStructureGeometryKHR. We need to put the
		// instances struct in a union and label it as instance data.
		_topASGeometry = {};
		_topASGeometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
		_topASGeometry.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR;
		_topASGeometry.geometry.instances = _instancesVk;

		_buildGeometryInfo = {};
		_buildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
		_buildGeometryInfo.flags = _flags;
		_buildGeometryInfo.geometryCount = 1;
		_buildGeometryInfo.pGeometries = &_topASGeometry;
		_buildGeometryInfo.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
		_buildGeometryInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
		_buildGeometryInfo.srcAccelerationStructure = nullptr;
		_buildGeometryInfo.pNext = nullptr;

		_buildSizesInfo = getBuildSizes(&instancesCount);
	}

	TopLevelAccelerationStructure::~TopLevelAccelerationStructure()
	{
	}

	void TopLevelAccelerationStructure::generate(
		VkCommandBuffer commandBuffer,
		std::shared_ptr<atta::vk::Buffer> resultBuffer,
		VkDeviceSize resultOffset,
		std::shared_ptr<atta::vk::Buffer> scratchBuffer,
		VkDeviceSize scratchOffset)
	{
		// Create the acceleration structure
		createAccelerationStructure(resultBuffer, resultOffset);

		// Build the actual bottom-level acceleration structure
		VkAccelerationStructureBuildRangeInfoKHR buildOffsetInfo = {};
		buildOffsetInfo.primitiveCount = _instancesCount;
		
		const VkAccelerationStructureBuildRangeInfoKHR* pBuildOffsetInfo = &buildOffsetInfo;

		_buildGeometryInfo.dstAccelerationStructure = handle();
		_buildGeometryInfo.scratchData.deviceAddress = scratchBuffer->getDeviceAddress() + scratchOffset;

		_deviceProcedures->vkCmdBuildAccelerationStructuresKHR(commandBuffer, 1, &_buildGeometryInfo, &pBuildOffsetInfo);
	}

	VkAccelerationStructureInstanceKHR TopLevelAccelerationStructure::createInstance(
		std::shared_ptr<BottomLevelAccelerationStructure> bottomLevelAs,
		const mat4& transform,
		const uint32_t instanceId,
		const uint32_t hitGroupId)
	{
		const auto& device = bottomLevelAs->getDevice();
		const auto& deviceProcedure = bottomLevelAs->getDeviceProcedures();

		VkAccelerationStructureDeviceAddressInfoKHR addressInfo = {};
		addressInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
		addressInfo.accelerationStructure = bottomLevelAs->handle();

		const VkDeviceAddress address = deviceProcedure->vkGetAccelerationStructureDeviceAddressKHR(device->handle(), &addressInfo);

		VkAccelerationStructureInstanceKHR instance = {};
		instance.instanceCustomIndex = instanceId;
		instance.mask = 0xFF; // The visibility mask is always set of 0xFF, but if some instances would need to be ignored in some cases, this flag should be passed by the application.
		instance.instanceShaderBindingTableRecordOffset = hitGroupId; // Set the hit group index, that will be used to find the shader code to execute when hitting the geometry.
		instance.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR; // Disable culling - more fine control could be provided by the application
		instance.accelerationStructureReference = address;

		// The instance.transform value only contains 12 values, corresponding to a 4x3 matrix,
		// hence saving the last row that is anyway always (0,0,0,1).
		// Since the matrix is row-major, we simply copy the first 12 values of the original 4x4 matrix
		std::memcpy(&instance.transform, &transform, sizeof(instance.transform));
		return instance;
	}

}
