//--------------------------------------------------
// Robot Simulator
// commandPool.h
// Date: 24/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef COMMAND_POOL_H
#define COMMAND_POOL_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"

class CommandPool
{
	public:
	CommandPool(Device* device);
	~CommandPool();

	VkCommandPool handle() const { return _commandPool; }
	Device* getDevice() const { return _device; }

	private:
    VkCommandPool _commandPool;
	Device* _device;
};

#endif// COMMAND_POOL_H
