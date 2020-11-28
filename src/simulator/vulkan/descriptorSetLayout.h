//--------------------------------------------------
// Robot Simulator
// descriptorSetLayout.h
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef DESCRIPTOR_SET_LAYOUT_H
#define DESCRIPTOR_SET_LAYOUT_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"
#include "descriptorBinding.h"

class DescriptorSetLayout
{
	public:
	DescriptorSetLayout(Device* device);
	DescriptorSetLayout(Device* device, std::vector<DescriptorBinding> descriptorBindings);
	~DescriptorSetLayout();

	VkDescriptorSetLayout handle() const { return _descriptorSetLayout; }
	Device* getDevice() const { return _device; }

	private:
    VkDescriptorSetLayout _descriptorSetLayout;
	Device* _device;
};

#endif// DESCRIPTOR_SET_LAYOUT_H
