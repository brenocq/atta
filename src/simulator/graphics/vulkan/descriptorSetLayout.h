//--------------------------------------------------
// Robot Simulator
// descriptorSetLayout.h
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_DESCRIPTOR_SET_LAYOUT_H
#define ATTA_VK_DESCRIPTOR_SET_LAYOUT_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"
#include "descriptorBinding.h"

namespace atta::vk
{
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
}

#endif// ATTA_VK_DESCRIPTOR_SET_LAYOUT_H
