#ifndef COMMAND_POOL_H
#define COMMAND_POOL_H

#include "device.h"

class CommandPool
{
	public:
	CommandPool(Device* device, uint32_t queueFamilyIndex, bool allowReset);
	~CommandPool();

	VkCommandPool handle() const { return _commandPool; }
	Device* device() const { return _device; }

	private:
	Device* _device;
	VkCommandPool _commandPool;
};

#endif// COMMAND_POOL_H
