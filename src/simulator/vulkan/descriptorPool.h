//--------------------------------------------------
// Robot Simulator
// descriptorPool.h
// Date: 07/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef DESCRIPTOR_POOL_H
#define DESCRIPTOR_POOL_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"

class DescriptorPool
{
	public:
	DescriptorPool(Device* device, std::vector<VkDescriptorPoolSize> poolSizes);
	~DescriptorPool();

	VkDescriptorPool handle() const { return _descriptorPool; }
	Device* getDevice() const { return _device; }

	private:
    VkDescriptorPool _descriptorPool;
	Device* _device;
};

#endif// DESCRIPTOR_POOL_H
