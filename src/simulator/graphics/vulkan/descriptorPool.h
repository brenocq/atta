//--------------------------------------------------
// Robot Simulator
// descriptorPool.h
// Date: 2020-07-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_DESCRIPTOR_POOL_H
#define ATTA_VK_DESCRIPTOR_POOL_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"
#include "descriptorBinding.h"

namespace atta::vk
{
	class DescriptorPool
	{
		public:
			DescriptorPool(std::shared_ptr<Device> device, std::vector<DescriptorBinding> descriptorBindings, size_t maxSets);
			~DescriptorPool();

			VkDescriptorPool handle() const { return _descriptorPool; }
			std::shared_ptr<Device> getDevice() const { return _device; }

		private:
			VkDescriptorPool _descriptorPool;
			std::shared_ptr<Device> _device;
	};
}

#endif// ATTA_VK_DESCRIPTOR_POOL_H
