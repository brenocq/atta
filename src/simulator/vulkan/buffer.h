//--------------------------------------------------
// Robot Simulator
// buffer.h
// Date: 06/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"
#include "commandPool.h"

class Buffer
{
	public:
	Buffer(Device* device, const int size, const VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	~Buffer();

	VkBuffer handle() { return _buffer; }
	Device* getDevice() const { return _device; }
	VkDeviceMemory getMemory() const { return _bufferMemory; }

	void copyFrom(CommandPool* commandPool, VkBuffer srcBuffer, VkDeviceSize size);

	protected:
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	
    VkBuffer _buffer;
	VkBufferCreateInfo _bufferInfo;
	VkDeviceMemory _bufferMemory;
	Device* _device;
};

#endif// BUFFER_H
