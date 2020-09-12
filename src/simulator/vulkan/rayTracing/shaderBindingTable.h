//--------------------------------------------------
// Robot Simulator
// shaderBindingTable.h
// Date: 2020-07-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef SHADER_BINDING_TABLE_H
#define SHADER_BINDING_TABLE_H

#include "../device.h"
#include "../buffer.h"
#include "deviceProcedures.h"
#include "rayTracingPipeline.h"

class ShaderBindingTable final
{
	public:
	struct Entry
	{
		uint32_t groupIndex;
		std::vector<unsigned char> inlineData;
	};

	ShaderBindingTable(
		Device* device,
		DeviceProcedures* deviceProcedures,
		RayTracingPipeline* rayTracingPipeline,
		VkPhysicalDeviceRayTracingPropertiesNV rayTracingProperties,
		const std::vector<Entry>& rayGenPrograms,
		const std::vector<Entry>& missPrograms,
		const std::vector<Entry>& hitGroups);

	~ShaderBindingTable();

	Buffer* getBuffer() const { return _buffer; }

	size_t getRayGenOffset() const { return _rayGenOffset; }
	size_t getMissOffset() const { return _missOffset; }
	size_t getHitGroupOffset() const { return _hitGroupOffset; }

	size_t getRayGenEntrySize() const { return _rayGenEntrySize; }
	size_t getMissEntrySize() const { return _missEntrySize; }
	size_t getHitGroupEntrySize() const { return _hitGroupEntrySize; }

	private:

	const size_t _rayGenEntrySize;
	const size_t _missEntrySize;
	const size_t _hitGroupEntrySize;

	const size_t _rayGenOffset;
	const size_t _missOffset;
	const size_t _hitGroupOffset;

	Device* _device;
 	Buffer* _buffer;
};

#endif// SHADER_BINDING_TABLE_H
