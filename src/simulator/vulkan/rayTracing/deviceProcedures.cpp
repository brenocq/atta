//--------------------------------------------------
// Robot Simulator
// deviceProcedures.cpp
// Date: 15/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <string>
#include "deviceProcedures.h"

template <class Func>
Func getProcedure(Device* device, const char* const name)
{
	const auto func = reinterpret_cast<Func>(vkGetDeviceProcAddr(device->handle(), name));
	if(func == nullptr)
	{
		std::cerr << BOLDRED << "[DeviceProcedures]" << RESET << RED << " Failed to get address of " << name << "!" << RESET << std::endl;
		exit(1);
	}
	return func;
}


DeviceProcedures::DeviceProcedures(Device* device):
	vkCreateAccelerationStructureNV(getProcedure<PFN_vkCreateAccelerationStructureNV>(device, "vkCreateAccelerationStructureNV")),
	vkDestroyAccelerationStructureNV(getProcedure<PFN_vkDestroyAccelerationStructureNV>(device, "vkDestroyAccelerationStructureNV")),
	vkGetAccelerationStructureMemoryRequirementsNV(getProcedure<PFN_vkGetAccelerationStructureMemoryRequirementsNV>(device, "vkGetAccelerationStructureMemoryRequirementsNV")),
	vkBindAccelerationStructureMemoryNV(getProcedure<PFN_vkBindAccelerationStructureMemoryNV>(device, "vkBindAccelerationStructureMemoryNV")),
	vkCmdBuildAccelerationStructureNV(getProcedure<PFN_vkCmdBuildAccelerationStructureNV>(device, "vkCmdBuildAccelerationStructureNV")),
	vkCmdCopyAccelerationStructureNV(getProcedure<PFN_vkCmdCopyAccelerationStructureNV>(device, "vkCmdCopyAccelerationStructureNV")),
	vkCmdTraceRaysNV(getProcedure<PFN_vkCmdTraceRaysNV>(device, "vkCmdTraceRaysNV")),
	vkCreateRayTracingPipelinesNV(getProcedure<PFN_vkCreateRayTracingPipelinesNV>(device, "vkCreateRayTracingPipelinesNV")),
	vkGetRayTracingShaderGroupHandlesNV(getProcedure<PFN_vkGetRayTracingShaderGroupHandlesNV>(device, "vkGetRayTracingShaderGroupHandlesNV")),
	vkGetAccelerationStructureHandleNV(getProcedure<PFN_vkGetAccelerationStructureHandleNV>(device, "vkGetAccelerationStructureHandleNV")),
	vkCmdWriteAccelerationStructuresPropertiesNV(getProcedure<PFN_vkCmdWriteAccelerationStructuresPropertiesNV>(device, "vkCmdWriteAccelerationStructuresPropertiesNV")),
	vkCompileDeferredNV(getProcedure<PFN_vkCompileDeferredNV>(device, "vkCompileDeferredNV"))
{
	_device = device;
}

DeviceProcedures::~DeviceProcedures()
{
}
