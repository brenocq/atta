//--------------------------------------------------
// Robot Simulator
// descriptorSetManager.h
// Date: 2020-07-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef DESCRIPTOR_SET_MANAGER_H
#define DESCRIPTOR_SET_MANAGER_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"
#include "descriptorBinding.h"
#include "descriptorPool.h"
#include "descriptorSetLayout.h"
#include "descriptorSets.h"

class DescriptorSetManager
{
	public:
		DescriptorSetManager(Device* device, std::vector<DescriptorBinding> descriptorBindings, size_t maxSets);
		~DescriptorSetManager();

		Device* getDevice() const { return _device; }
		DescriptorPool* getDescriptorPool() const { return _descriptorPool; }
		DescriptorSetLayout* getDescriptorSetLayout() const { return _descriptorSetLayout; }
		DescriptorSets* getDescriptorSets() const { return _descriptorSets; }

	private:
		Device* _device;
		DescriptorPool* _descriptorPool;
		DescriptorSetLayout* _descriptorSetLayout;
		DescriptorSets* _descriptorSets;
};

#endif// DESCRIPTOR_SET_MANAGER_H
