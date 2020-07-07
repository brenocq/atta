//--------------------------------------------------
// Robot Simulator
// descriptorSetLayout.h
// Date: 06/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef DESCRIPTOR_SET_LAYOUT_H
#define DESCRIPTOR_SET_LAYOUT_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"

class DescriptorSetLayout
{
	public:
	DescriptorSetLayout(Device* device);
	~DescriptorSetLayout();

	VkDescriptorSetLayout handle() const { return _descriptorSetLayout; }
	Device* getDevice() const { return _device; }

	private:
    VkDescriptorSetLayout _descriptorSetLayout;
	Device* _device;
};

#endif// DESCRIPTOR_SET_LAYOUT_H
