#ifndef UNIFORM_BUFFER_H
#define UNIFORM_BUFFER_H

#include <memory>
#include "glm.h"
#include "../vulkan/buffer.h"
#include "../vulkan/device.h"
#include "../vulkan/deviceMemory.h"

class UniformBufferObject
{
public:

	glm::mat4 modelView;
	glm::mat4 projection;
	glm::mat4 modelViewInverse;
	glm::mat4 projectionInverse;
	float aperture;
	float focusDistance;
	uint32_t totalNumberOfSamples;
	uint32_t numberOfSamples;
	uint32_t numberOfBounces;
	uint32_t randomSeed;
	uint32_t gammaCorrection; // bool
	uint32_t hasSky; // bool
};

class UniformBuffer
{
public:
	UniformBuffer(const UniformBuffer&) = delete;
	UniformBuffer& operator = (const UniformBuffer&) = delete;
	UniformBuffer& operator = (UniformBuffer&&) = delete;

	explicit UniformBuffer(Device* device);
	UniformBuffer(UniformBuffer&& other) noexcept;
	~UniformBuffer();

	Buffer* buffer() const { return _buffer; }

	void setValue(const UniformBufferObject& ubo);

private:

	Buffer* _buffer;
	DeviceMemory* _memory;
};

#endif// UNIFORM_BUFFER_H
