#ifndef DESCRIPTOR_POOL_H
#define DESCRIPTOR_POOL_H

#include <vector>
#include "device.h"
#include "descriptorBinding.h"

class DescriptorPool
{
	public:
		DescriptorPool(Device* device, const std::vector<DescriptorBinding>& descriptorBindings, size_t maxSets);
		~DescriptorPool();

		VkDescriptorPool handle() const { return _descriptorPool; }
		Device* device() const { return _device; }

	private:
		Device* _device;
		VkDescriptorPool _descriptorPool;
};

#endif// DESCRIPTOR_POOL_H
