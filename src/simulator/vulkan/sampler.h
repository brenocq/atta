//--------------------------------------------------
// Robot Simulator
// sampler.h
// Date: 08/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef SAMPLER_H
#define SAMPLER_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"

class Sampler
{
	public:
	Sampler(Device* device, uint32_t mipLevels=1);
	~Sampler();

	Device* getDevice() const { return _device; }
	VkSampler handle() const { return _sampler; }

	private:

	Device* _device;
	VkSampler _sampler;
};

#endif// SAMPLER_H
