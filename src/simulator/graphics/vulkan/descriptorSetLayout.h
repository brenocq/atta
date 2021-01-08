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
	DescriptorSetLayout(std::shared_ptr<Device> device);
	DescriptorSetLayout(std::shared_ptr<Device> device, std::vector<DescriptorBinding> descriptorBindings);
	~DescriptorSetLayout();

	VkDescriptorSetLayout handle() const { return _descriptorSetLayout; }
	std::shared_ptr<Device> getDevice() const { return _device; }

	private:
    VkDescriptorSetLayout _descriptorSetLayout;
	std::shared_ptr<Device> _device;
};

#endif// DESCRIPTOR_SET_LAYOUT_H
