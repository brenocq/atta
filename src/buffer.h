#ifndef BUFFER_H
#define BUFFER_H

#include "device.h"
#include "deviceMemory.h"
#include "commandPool.h"

class Buffer
{
	public:
		Buffer(const Device* device, size_t size, VkBufferUsageFlags usage);
		~Buffer();

		VkBuffer handle() const { return _buffer; }
		Device* device() const { return _device; }

		DeviceMemory allocateMemory(VkMemoryPropertyFlags properties);
		VkMemoryRequirements getMemoryRequirements() const;

		void copyFrom(CommandPool* commandPool, const Buffer& src, VkDeviceSize size);

	private:
		Device* _device;
		VkBuffer _buffer;
};

#endif// BUFFER_H
