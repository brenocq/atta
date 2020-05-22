#ifndef DESCRIPTOR_SET_LAYOUT_H
#define DESCRIPTOR_SET_LAYOUT_H

#include <vector>
#include "device.h"
#include "descriptorBinding.h"

class DescriptorSetLayout
{
	public:
		DescriptorSetLayout(Device* device, const std::vector<DescriptorBinding>& descriptorBindings);
		~DescriptorSetLayout();

		VkDescriptorSetLayout handle() const { return _layout; }

	private:
		Device* _device;
		VkDescriptorSetLayout _layout;
};

#endif// DESCRIPTOR_SET_LAYOUT_H
