//--------------------------------------------------
// Robot Simulator
// fence.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef FENCE_H
#define FENCE_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"

class Fence
{
	public:
	Fence(Device* device);
	~Fence();

	VkFence handle() { return _fence; }
	Device* getDevice() const { return _device; }

	void reset();
	void wait(uint64_t timeout) const;

	private:
    VkFence _fence;
	Device* _device;
};

#endif// FENCE_H
