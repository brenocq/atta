//--------------------------------------------------
// Robot Simulator
// sampler.h
// Date: 2020-07-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_SAMPLER_H
#define ATTA_VK_SAMPLER_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"

namespace atta::vk
{
	class Sampler
	{
		public:
			Sampler(std::shared_ptr<Device> device, uint32_t mipLevels=1, bool unnormalizedCoordinates=false);
			~Sampler();

			std::shared_ptr<Device> getDevice() const { return _device; }
			VkSampler handle() const { return _sampler; }

		private:
			std::shared_ptr<Device> _device;
			VkSampler _sampler;
	};
}

#endif// ATTA_VK_SAMPLER_H
