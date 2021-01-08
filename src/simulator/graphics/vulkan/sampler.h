//--------------------------------------------------
// Robot Simulator
// sampler.h
// Date: 2020-07-08
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
	Sampler(std::shared_ptr<Device> device, uint32_t mipLevels=1);
	~Sampler();

	std::shared_ptr<Device> getDevice() const { return _device; }
	VkSampler handle() const { return _sampler; }

	private:

	std::shared_ptr<Device> _device;
	VkSampler _sampler;
};

#endif// SAMPLER_H
