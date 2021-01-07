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
		Fence(std::shared_ptr<Device> device);
		~Fence();

		VkFence handle() { return _fence; }
		std::shared_ptr<Device> getDevice() const { return _device; }

		void reset();
		void wait(uint64_t timeout) const;

	private:
		VkFence _fence;
		std::shared_ptr<Device> _device;
};

#endif// FENCE_H
