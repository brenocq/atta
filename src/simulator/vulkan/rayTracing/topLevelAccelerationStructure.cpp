//--------------------------------------------------
// Robot Simulator
// topLevelAcceletationStructure.cpp
// Date: 22/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "topLevelAccelerationStructure.h"

VkAccelerationStructureCreateInfoNV getCreateInfo(const size_t instanceCount, const bool allowUpdate)
{
	const auto flags = allowUpdate
		? VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_NV
		: VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_NV;

	VkAccelerationStructureCreateInfoNV structureInfo = {};
	structureInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_NV;
	structureInfo.pNext = nullptr;
	structureInfo.info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_INFO_NV;
	structureInfo.info.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_NV;
	structureInfo.info.flags = flags;
	structureInfo.compactedSize = 0;
	structureInfo.info.instanceCount = static_cast<uint32_t>(instanceCount);
	structureInfo.info.geometryCount = 0; // Since this is a top-level AS, it does not contain any geometry
	structureInfo.info.pGeometries = nullptr;

	return structureInfo;
}

TopLevelAccelerationStructure::TopLevelAccelerationStructure(
	DeviceProcedures* deviceProcedures,
	const std::vector<VkGeometryInstance>& geometryInstances,
	const bool allowUpdate) :
	AccelerationStructure(deviceProcedures, getCreateInfo(geometryInstances.size(), allowUpdate)),
	_geometryInstances(geometryInstances)
{
}

TopLevelAccelerationStructure::~TopLevelAccelerationStructure()
{
}

void TopLevelAccelerationStructure::generate(
	VkCommandBuffer commandBuffer,
	Buffer* resultBuffer,
	VkDeviceSize resultOffset,
	Buffer* scratchBuffer,
	VkDeviceSize scratchOffset,
	Buffer* instanceBuffer,
	VkDeviceSize instanceOffset,
	bool updateOnly) const
{
	if (updateOnly && !_allowUpdate)
	{
		std::cerr << BOLDYELLOW << "[TopLevelAccelerationStructure]" << YELLOW << " Cannot update readonly structure!" << RESET << std::endl;
		return;
	}

	const VkAccelerationStructureNV previousStructure = updateOnly ? handle() : nullptr;

	// Copy the instance descriptors into the provider buffer.
	const auto instancesBufferSize = _geometryInstances.size() * sizeof(VkGeometryInstance);

	void* data = instanceBuffer->mapMemory(0, instancesBufferSize);
	std::memcpy(data, _geometryInstances.data(), instancesBufferSize);

	// Bind the acceleration structure descriptor to the actual memory that will contain it
	VkBindAccelerationStructureMemoryInfoNV bindInfo = {};
	bindInfo.sType = VK_STRUCTURE_TYPE_BIND_ACCELERATION_STRUCTURE_MEMORY_INFO_NV;
	bindInfo.pNext = nullptr;
	bindInfo.accelerationStructure = handle();
	bindInfo.memory = resultBuffer->getMemory();
	bindInfo.memoryOffset = resultOffset;
	bindInfo.deviceIndexCount = 0;
	bindInfo.pDeviceIndices = nullptr;

	if(_deviceProcedures->vkBindAccelerationStructureMemoryNV(getDevice()->handle(), 1, &bindInfo) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[TopLevelAccelerationStructure]" << RESET << RED << " Failed to bind acceleration structure!" << RESET << std::endl;
		exit(1);
	}

	// Build the actual bottom-level acceleration structure
	const auto flags = _allowUpdate
		? VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_NV 
		: VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_NV;

	VkAccelerationStructureInfoNV buildInfo = {};
	buildInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_INFO_NV;
	buildInfo.pNext = nullptr;
	buildInfo.flags = flags;
	buildInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_NV;
	buildInfo.instanceCount = static_cast<uint32_t>(_geometryInstances.size());
	buildInfo.geometryCount = 0;
	buildInfo.pGeometries = nullptr;

	_deviceProcedures->vkCmdBuildAccelerationStructureNV(
		commandBuffer, &buildInfo, instanceBuffer->handle(), instanceOffset, updateOnly, handle(), previousStructure, scratchBuffer->handle(), scratchOffset);
}

VkGeometryInstance TopLevelAccelerationStructure::createGeometryInstance(
	const BottomLevelAccelerationStructure& bottomLevelAs,
	const glm::mat4& transform,
	const uint32_t instanceId,
	const uint32_t hitGroupIndex)
{
	const auto& device = bottomLevelAs.getDevice();
	DeviceProcedures* deviceProcedures = bottomLevelAs.getDeviceProcedures();

	uint64_t accelerationStructureHandle;
	if(deviceProcedures->vkGetAccelerationStructureHandleNV(device->handle(), bottomLevelAs.handle(), sizeof(uint64_t), &accelerationStructureHandle) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[TopLevelAccelerationStructure]" << RESET << RED << " Failed to get acceleration structure handle!" << RESET << std::endl;
		exit(1);
	}

	VkGeometryInstance geometryInstance = {};
	std::memcpy(geometryInstance.transform, &transform, sizeof(glm::mat4));
	geometryInstance.instanceCustomIndex = instanceId;
	geometryInstance.mask = 0xFF; // The visibility mask is always set of 0xFF, but if some instances would need to be ignored in some cases, this flag should be passed by the application.
	geometryInstance.instanceOffset = hitGroupIndex; // Set the hit group index, that will be used to find the shader code to execute when hitting the geometry.
	geometryInstance.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_CULL_DISABLE_BIT_NV; // Disable culling - more fine control could be provided by the application
	geometryInstance.accelerationStructureHandle = accelerationStructureHandle;

	return geometryInstance;
}

