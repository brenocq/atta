//--------------------------------------------------
// Atta Ray Tracing Vulkan
// bottomLevelAcceletationStructure.cpp
// Date: 2020-07-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "bottomLevelAccelerationStructure.h"
#include "simulator/graphics/core/vertex.h"
#include "simulator/helpers/log.h"

namespace atta::rt::vk
{
	BottomLevelAccelerationStructure::BottomLevelAccelerationStructure(
			std::shared_ptr<DeviceProcedures> deviceProcedures, 
			std::shared_ptr<RayTracingProperties> rayTracingProperties, 
			std::shared_ptr<BottomLevelGeometry> geometries):
		AccelerationStructure(deviceProcedures, rayTracingProperties),
		_geometries(geometries)
	{
		_buildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
		_buildGeometryInfo.pNext = nullptr;
		_buildGeometryInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
		_buildGeometryInfo.flags = _flags;
		_buildGeometryInfo.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
		_buildGeometryInfo.srcAccelerationStructure = nullptr;
		_buildGeometryInfo.dstAccelerationStructure = nullptr;
		_buildGeometryInfo.geometryCount = geometries->size();
		_buildGeometryInfo.pGeometries = _geometries->geometry().data();
		_buildGeometryInfo.ppGeometries = nullptr;
		_buildGeometryInfo.scratchData = {};
		

		std::vector<uint32_t> maxPrimCount(_geometries->buildOffsetInfo().size());

		for(size_t i = 0; i != maxPrimCount.size(); i++)
		{
			maxPrimCount[i] = _geometries->buildOffsetInfo()[i].primitiveCount;
		}
		
		_buildSizesInfo = getBuildSizes(maxPrimCount.data());
	}

	BottomLevelAccelerationStructure::~BottomLevelAccelerationStructure()
	{

	}

	void BottomLevelAccelerationStructure::generate(
		VkCommandBuffer commandBuffer,
		std::shared_ptr<atta::vk::Buffer> resultBuffer,
		VkDeviceSize resultOffset,
		std::shared_ptr<atta::vk::Buffer> scratchBuffer,
		VkDeviceSize scratchOffset)
	{
		// Create the acceleration structure.
		createAccelerationStructure(resultBuffer, resultOffset);

		// Build the actual bottom-level acceleration structure
		const VkAccelerationStructureBuildRangeInfoKHR* pBuildOffsetInfo = _geometries->buildOffsetInfo().data();

		_buildGeometryInfo.dstAccelerationStructure = handle();
		_buildGeometryInfo.scratchData.deviceAddress = scratchBuffer->getDeviceAddress() + scratchOffset;

		_deviceProcedures->vkCmdBuildAccelerationStructuresKHR(commandBuffer, 1, &_buildGeometryInfo, &pBuildOffsetInfo);
	}
}
