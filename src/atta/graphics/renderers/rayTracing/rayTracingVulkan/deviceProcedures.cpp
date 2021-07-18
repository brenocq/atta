//--------------------------------------------------
// Atta Ray Tracing Vulkan
// deviceProcedures.cpp
// Date: 2020-07-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <string>
#include <atta/graphics/renderers/rayTracing/rayTracingVulkan/deviceProcedures.h>
#include <atta/helpers/log.h>

namespace atta::rt::vk
{
	template <class Func>
	Func getProcedure(std::shared_ptr<atta::vk::Device> device, const char* const name)
	{
		const auto func = reinterpret_cast<Func>(vkGetDeviceProcAddr(device->handle(), name));
		if(func == nullptr)
		{
			Log::error("rt::vk::DeviceProcedures", "Failed to get address of $0!", name);
			exit(1);
		}
		return func;
	}


	DeviceProcedures::DeviceProcedures(std::shared_ptr<atta::vk::Device> device):
		vkCreateAccelerationStructureKHR(getProcedure<PFN_vkCreateAccelerationStructureKHR>(device, "vkCreateAccelerationStructureKHR")),
		vkDestroyAccelerationStructureKHR(getProcedure<PFN_vkDestroyAccelerationStructureKHR>(device, "vkDestroyAccelerationStructureKHR")),
		vkGetAccelerationStructureBuildSizesKHR(getProcedure<PFN_vkGetAccelerationStructureBuildSizesKHR>(device, "vkGetAccelerationStructureBuildSizesKHR")),
		vkCmdBuildAccelerationStructuresKHR(getProcedure<PFN_vkCmdBuildAccelerationStructuresKHR>(device, "vkCmdBuildAccelerationStructuresKHR")),
		vkCmdCopyAccelerationStructureKHR(getProcedure<PFN_vkCmdCopyAccelerationStructureKHR>(device, "vkCmdCopyAccelerationStructureKHR")),
		vkCmdTraceRaysKHR(getProcedure<PFN_vkCmdTraceRaysKHR>(device, "vkCmdTraceRaysKHR")),
		vkCreateRayTracingPipelinesKHR(getProcedure<PFN_vkCreateRayTracingPipelinesKHR>(device, "vkCreateRayTracingPipelinesKHR")),
		vkGetRayTracingShaderGroupHandlesKHR(getProcedure<PFN_vkGetRayTracingShaderGroupHandlesKHR>(device, "vkGetRayTracingShaderGroupHandlesKHR")),
		vkGetAccelerationStructureDeviceAddressKHR(getProcedure<PFN_vkGetAccelerationStructureDeviceAddressKHR>(device, "vkGetAccelerationStructureDeviceAddressKHR")),
		vkCmdWriteAccelerationStructuresPropertiesKHR(getProcedure<PFN_vkCmdWriteAccelerationStructuresPropertiesKHR>(device, "vkCmdWriteAccelerationStructuresPropertiesKHR")),
		vkGetDeferredOperationMaxConcurrencyKHR(getProcedure<PFN_vkGetDeferredOperationMaxConcurrencyKHR>(device, "vkGetDeferredOperationMaxConcurrencyKHR")),
		vkCreateDeferredOperationKHR(getProcedure<PFN_vkCreateDeferredOperationKHR>(device, "vkCreateDeferredOperationKHR")),
		vkDeferredOperationJoinKHR(getProcedure<PFN_vkDeferredOperationJoinKHR>(device, "vkDeferredOperationJoinKHR")),
		vkGetDeferredOperationResultKHR(getProcedure<PFN_vkGetDeferredOperationResultKHR>(device, "vkGetDeferredOperationResultKHR")),
		vkDestroyDeferredOperationKHR(getProcedure<PFN_vkDestroyDeferredOperationKHR>(device, "vkDestroyDeferredOperationKHR")),
		_device(device)
	{
	}

	DeviceProcedures::~DeviceProcedures()
	{
	}
}
