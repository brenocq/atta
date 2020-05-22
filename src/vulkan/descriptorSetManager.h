#ifndef DESCRIPTOR_SET_MANAGER_H
#define DESCRIPTOR_SET_MANAGER_H

#include <vector>
#include "device.h"
#include "descriptorBinding.h"
#include "descriptorSets.h"
#include "descriptorPool.h"

class DescriptorSetManager
{
	public:
		DescriptorSetManager(Device* device, const std::vector<DescriptorBinding>& descriptorBindings, size_t maxSets);
		~DescriptorSetManager();

		DescriptorSetLayout* descriptorSetLayout() const { return _descriptorSetLayout; }
		DescriptorSets* descriptorSets() const { return _descriptorSets; }

	private:

		DescriptorPool* _descriptorPool;
		DescriptorSetLayout* _descriptorSetLayout;
		DescriptorSets* _descriptorSets;
};

#endif// DESCRIPTOR_SET_MANAGER_H
