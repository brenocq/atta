//--------------------------------------------------
// Robot Simulator
// shaderBindingTable.cpp
// Date: 23/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "shaderBindingTable.h"

size_t roundUp(size_t size, size_t powerOf2Alignment)
{
	return (size+powerOf2Alignment-1) & ~(powerOf2Alignment-1);
}

size_t getEntrySize(VkPhysicalDeviceRayTracingPropertiesNV rayTracingProperties, const std::vector<ShaderBindingTable::Entry>& entries)
{
	// Find the maximum number of parameters used by a single entry
	size_t maxArgs = 0;

	for (const auto& entry : entries)
	{
		maxArgs = std::max(maxArgs, entry.inlineData.size());
	}

	// A SBT entry is made of a program ID and a set of 4-byte parameters (offsets or push constants)
	// and must be 16-bytes-aligned.
	// Changed from 16 to 64 (base alignment on my GPU)
	return roundUp(rayTracingProperties.shaderGroupHandleSize + maxArgs, 64);
}

size_t copyShaderData(
	uint8_t* const dst, 
	VkPhysicalDeviceRayTracingPropertiesNV rayTracingProperties,
	const std::vector<ShaderBindingTable::Entry>& entries, 
	const size_t entrySize, 
	const uint8_t* const shaderHandleStorage)
{
	const auto handleSize = rayTracingProperties.shaderGroupHandleSize;

	uint8_t* pDst = dst;

	for (const auto& entry : entries)
	{
		// Copy the shader identifier that was previously obtained with vkGetRayTracingShaderGroupHandlesNV.
		std::memcpy(pDst, shaderHandleStorage + entry.groupIndex * handleSize, handleSize);
		std::memcpy(pDst + handleSize, entry.inlineData.data(), entry.inlineData.size());

		pDst += entrySize;
	}

	return entries.size() * entrySize;
}

ShaderBindingTable::ShaderBindingTable(
	Device* device, 
	DeviceProcedures* deviceProcedures, 
	RayTracingPipeline* rayTracingPipeline,
	VkPhysicalDeviceRayTracingPropertiesNV rayTracingProperties,
	const std::vector<Entry>& rayGenPrograms,
	const std::vector<Entry>& missPrograms, 
	const std::vector<Entry>& hitGroups):
	_rayGenEntrySize(getEntrySize(rayTracingProperties, rayGenPrograms)),
	_missEntrySize(getEntrySize(rayTracingProperties, missPrograms)),
	_hitGroupEntrySize(getEntrySize(rayTracingProperties, hitGroups)),
	_rayGenOffset(0),
	_missOffset(rayGenPrograms.size() * _rayGenEntrySize),
	_hitGroupOffset(_missOffset + missPrograms.size() * _missEntrySize)
{
	_device = device;

	// Compute the size of the table.
	const size_t sbtSize =
		rayGenPrograms.size() * _rayGenEntrySize +
		missPrograms.size() * _missEntrySize +
		hitGroups.size() * _hitGroupEntrySize;

	// Allocate buffer & memory.
	_buffer = new Buffer(_device, sbtSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

	// Generate the table.
	const uint32_t handleSize = rayTracingProperties.shaderGroupHandleSize;
	const size_t groupCount = rayGenPrograms.size() + missPrograms.size() + hitGroups.size();
	std::vector<uint8_t> shaderHandleStorage(groupCount * handleSize);

	if(deviceProcedures->vkGetRayTracingShaderGroupHandlesNV(
		_device->handle(), 
		rayTracingPipeline->handle(), 
		0, static_cast<uint32_t>(groupCount),
		shaderHandleStorage.size(),
		shaderHandleStorage.data()) != VK_SUCCESS)
	{
		std::cerr << BOLDRED << "[ShaderBindingTable]" << RESET << RED << " Failed to get ray tracing shader group handles" << RESET << std::endl;
		exit(1);
	}

	// Copy the shader identifiers followed by their resource pointers or root constants: 
	// first the ray generation, then the miss shaders, and finally the set of hit groups.
	auto pData = static_cast<uint8_t*>(_buffer->mapMemory(0, sbtSize));

	pData += copyShaderData(pData, rayTracingProperties, rayGenPrograms, _rayGenEntrySize, shaderHandleStorage.data());
	pData += copyShaderData(pData, rayTracingProperties, missPrograms, _missEntrySize, shaderHandleStorage.data());
	         copyShaderData(pData, rayTracingProperties, hitGroups, _hitGroupEntrySize, shaderHandleStorage.data());

	_buffer->unmapMemory();
}

ShaderBindingTable::~ShaderBindingTable()
{
	delete _buffer;
	_buffer = nullptr;
}
