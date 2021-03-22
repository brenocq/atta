//--------------------------------------------------
// Robot Simulator
// descriptorSetLayout.h
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VULKAN_DESCRIPTOR_SET_LAYOUT_H
#define ATTA_GRAPHICS_VULKAN_DESCRIPTOR_SET_LAYOUT_H

#include <iostream>
#include <string.h>
#include <atta/graphics/vulkan/device.h>
#include <atta/graphics/vulkan/descriptorBinding.h>

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

#endif// ATTA_GRAPHICS_VULKAN_DESCRIPTOR_SET_LAYOUT_H
