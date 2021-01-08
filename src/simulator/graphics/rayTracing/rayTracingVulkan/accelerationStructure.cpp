//--------------------------------------------------
// Robot Simulator
// acceletationStructure.cpp
// Date: 2020-07-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "accelerationStructure.h"

AccelerationStructure::AccelerationStructure(DeviceProcedures* deviceProcedures, const VkAccelerationStructureCreateInfoNV& createInfo):
	_allowUpdate(createInfo.info.flags & VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_NV)
{
	_deviceProcedures = deviceProcedures;
	_device = _deviceProcedures->getDevice();


	if(_deviceProcedures->vkCreateAccelerationStructureNV(_device->handle(), &createInfo, nullptr, &_accelerationStructure) != VK_SUCCESS)
	{
		std::cerr << BOLDRED << "[AccelerationStructure]" << RESET << RED << " Failed to create acceleration strucutre!" << RESET << std::endl;
		exit(1);
	}
}

AccelerationStructure::~AccelerationStructure()
{
	if (_accelerationStructure != nullptr)
	{
		_deviceProcedures->vkDestroyAccelerationStructureNV(_device->handle(), _accelerationStructure, nullptr);
		_accelerationStructure = nullptr;
	}
}

AccelerationStructure::MemoryRequirements AccelerationStructure::getMemoryRequirements() const
{
	VkAccelerationStructureMemoryRequirementsInfoNV memoryRequirementsInfo{};
	memoryRequirementsInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_INFO_NV;
	memoryRequirementsInfo.pNext = nullptr;
	memoryRequirementsInfo.accelerationStructure = _accelerationStructure;

	// If the descriptor already contains the geometry info, so we can directly compute the estimated size and required scratch memory.
	VkMemoryRequirements2 memoryRequirements = {};
	memoryRequirementsInfo.type = VK_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_TYPE_OBJECT_NV;
	_deviceProcedures->vkGetAccelerationStructureMemoryRequirementsNV(_device->handle(), &memoryRequirementsInfo, &memoryRequirements);
	const auto resultRequirements = memoryRequirements.memoryRequirements;

	memoryRequirementsInfo.type = VK_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_TYPE_BUILD_SCRATCH_NV;
	_deviceProcedures->vkGetAccelerationStructureMemoryRequirementsNV(_device->handle(), &memoryRequirementsInfo, &memoryRequirements);
	const auto buildRequirements = memoryRequirements.memoryRequirements;

	memoryRequirementsInfo.type = VK_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_TYPE_UPDATE_SCRATCH_NV;
	_deviceProcedures->vkGetAccelerationStructureMemoryRequirementsNV(_device->handle(), &memoryRequirementsInfo, &memoryRequirements);
	const auto updateRequirements = memoryRequirements.memoryRequirements;

	return { resultRequirements, buildRequirements, updateRequirements };
}

void AccelerationStructure::memoryBarrier(VkCommandBuffer commandBuffer)
{
	// Wait for the builder to complete by setting a barrier on the resulting buffer. This is
	// particularly important as the construction of the top-level hierarchy may be called right
	// afterwards, before executing the command list.
	VkMemoryBarrier memoryBarrier = {};
	memoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
	memoryBarrier.pNext = nullptr;
	memoryBarrier.srcAccessMask = VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_NV | VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_NV;
	memoryBarrier.dstAccessMask = VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_NV | VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_NV;

	vkCmdPipelineBarrier(
		commandBuffer,
		VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_NV,
		VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_NV,
		0, 1, &memoryBarrier, 0, nullptr, 0, nullptr);
}
