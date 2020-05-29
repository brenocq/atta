#include "uniformBuffer.h"
#include <cstring>

UniformBuffer::UniformBuffer(Device* device)
{
	const auto bufferSize = sizeof(UniformBufferObject);

	_buffer = new Buffer(device, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
	_memory = new DeviceMemory(_buffer->allocateMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
}

UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept :
	_buffer(other._buffer),
	_memory(other._memory)
{
	other._buffer = nullptr;
	other._memory = nullptr;
}

UniformBuffer::~UniformBuffer()
{
	delete _buffer;
	delete _memory;// release memory after bound buffer has been destroyed
}

void UniformBuffer::setValue(const UniformBufferObject& ubo)
{
	const auto data = _memory->mapMemory(0, sizeof(UniformBufferObject));
	std::memcpy(data, &ubo, sizeof(ubo));
	_memory->unmapMemory();
}
