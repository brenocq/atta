#ifndef DEVICE_MEMORY_H
#define DEVICE_MEMORY_H

#include "device.h"

class DeviceMemory
{
	public:

		DeviceMemory(const DeviceMemory&) = delete;
		DeviceMemory& operator = (const DeviceMemory&) = delete;
		DeviceMemory& operator = (DeviceMemory&&) = delete;

		DeviceMemory(const Device* device, size_t size, uint32_t memoryTypeBits, VkMemoryPropertyFlags properties);
		DeviceMemory(DeviceMemory&& other) noexcept;
		~DeviceMemory();

		VkDeviceMemory handle() const { return _memory; }
		Device* device() const { return _device; }

		void* mapMemory(size_t offset, size_t size);
		void unmapMemory();

	private:
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

		Device* _device;
		VkDeviceMemory _memory;
};

#endif// DEVICE_MEMORY_H
