//--------------------------------------------------
// Robot Simulator
// buffer.h
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_BUFFER_H
#define ATTA_VK_BUFFER_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"
#include "commandPool.h"

namespace atta::vk
{
	class Buffer
	{
		public:
			Buffer(std::shared_ptr<Device> device, const int size, const VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
			~Buffer();

			VkBuffer handle() { return _buffer; }
			std::shared_ptr<Device> getDevice() const { return _device; }
			VkDeviceMemory getMemory() const { return _bufferMemory; }

			void copyFrom(std::shared_ptr<CommandPool> commandPool, VkBuffer srcBuffer, VkDeviceSize size);

			void* mapMemory(const size_t offset, const size_t size);
			void unmapMemory();
		protected:
			uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
			
			VkBuffer _buffer;
			VkBufferCreateInfo _bufferInfo;
			VkDeviceMemory _bufferMemory;
			std::shared_ptr<Device> _device;
	};
}

#endif// ATTA_VK_BUFFER_H
