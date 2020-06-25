//--------------------------------------------------
// Robot Simulator
// semaphore.h
// Date: 24/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"

class Semaphore
{
	public:
	Semaphore(Device* device);
	~Semaphore();

	VkSemaphore handle() const { return _semaphore; }
	Device* getDevice() const { return _device; }

	private:
    VkSemaphore _semaphore;
	Device* _device;
};

#endif// SEMAPHORE_H
