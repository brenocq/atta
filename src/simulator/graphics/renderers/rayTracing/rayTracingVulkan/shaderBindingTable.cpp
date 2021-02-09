//--------------------------------------------------
// Atta Ray Tracing Vulkan
// shaderBindingTable.cpp
// Date: 2020-07-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "shaderBindingTable.h"
#include "simulator/helpers/log.h"
#include <cstring>

namespace atta::rt::vk
{
	size_t roundUpSh(size_t size, size_t powerOf2Alignment)
	{
		return (size+powerOf2Alignment-1) & ~(powerOf2Alignment-1);
	}

	size_t getEntrySize(std::shared_ptr<RayTracingProperties> rayTracingProperties, const std::vector<ShaderBindingTable::Entry>& entries)
	{
		// Find the maximum number of parameters used by a single entry
		size_t maxArgs = 0;

		for (const auto& entry : entries)
		{
			maxArgs = std::max(maxArgs, entry.inlineData.size());
		}

		// A SBT entry is made of a program ID and a set of 4-byte parameters (see shaderRecordEXT)
		// Its size is ShaderGroupHandleSize (plus parameters) and must be aligned to ShaderGroupBaseAlignment
		return roundUpSh(rayTracingProperties->shaderGroupHandleSize() + maxArgs, rayTracingProperties->shaderGroupBaseAlignment());
	}

	size_t copyShaderData(
		uint8_t* const dst, 
		std::shared_ptr<RayTracingProperties> rayTracingProperties,
		const std::vector<ShaderBindingTable::Entry>& entries, 
		const size_t entrySize, 
		const uint8_t* const shaderHandleStorage)
	{
		const auto handleSize = rayTracingProperties->shaderGroupHandleSize();

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
		std::shared_ptr<atta::vk::Device> device, 
		std::shared_ptr<DeviceProcedures> deviceProcedures, 
		std::shared_ptr<RayTracingPipeline> rayTracingPipeline,
		std::shared_ptr<RayTracingProperties> rayTracingProperties,
		const std::vector<Entry>& rayGenPrograms,
		const std::vector<Entry>& missPrograms, 
		const std::vector<Entry>& hitGroups):
		_device(device),

		_rayGenEntrySize(getEntrySize(rayTracingProperties, rayGenPrograms)),
		_missEntrySize(getEntrySize(rayTracingProperties, missPrograms)),
		_hitGroupEntrySize(getEntrySize(rayTracingProperties, hitGroups)),

		_rayGenOffset(0),
		_missOffset(rayGenPrograms.size() * _rayGenEntrySize),
		_hitGroupOffset(_missOffset + missPrograms.size() * _missEntrySize),

		_rayGenSize(rayGenPrograms.size() * _rayGenEntrySize),
		_missSize(missPrograms.size() * _missEntrySize),
		_hitGroupSize(hitGroups.size() * _hitGroupEntrySize)
	{
		// Compute the size of the table.
		const size_t sbtSize =
			rayGenPrograms.size() * _rayGenEntrySize +
			missPrograms.size() * _missEntrySize +
			hitGroups.size() * _hitGroupEntrySize;

		// Allocate buffer & memory.
		_buffer = std::make_shared<atta::vk::Buffer>(_device, sbtSize, 
				VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
				VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT
				);

		// Generate the table.
		const uint32_t handleSize = rayTracingProperties->shaderGroupHandleSize();
		const size_t groupCount = rayGenPrograms.size() + missPrograms.size() + hitGroups.size();
		std::vector<uint8_t> shaderHandleStorage(groupCount * handleSize);

		if(deviceProcedures->vkGetRayTracingShaderGroupHandlesKHR(
			_device->handle(), 
			rayTracingPipeline->handle(), 
			0, static_cast<uint32_t>(groupCount),
			shaderHandleStorage.size(),
			shaderHandleStorage.data()) != VK_SUCCESS)
		{
			Log::error("ShaderBindingTable", "Failed to get ray tracing shader group handles!");
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
	}
}
