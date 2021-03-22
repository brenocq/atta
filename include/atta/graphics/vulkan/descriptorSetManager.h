//--------------------------------------------------
// Robot Simulator
// descriptorSetManager.h
// Date: 2020-07-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VULKAN_DESCRIPTOR_SET_MANAGER_H
#define ATTA_GRAPHICS_VULKAN_DESCRIPTOR_SET_MANAGER_H

#include <string>
#include <memory>
#include <atta/graphics/vulkan/device.h>
#include <atta/graphics/vulkan/descriptorBinding.h>
#include <atta/graphics/vulkan/descriptorPool.h>
#include <atta/graphics/vulkan/descriptorSetLayout.h>
#include <atta/graphics/vulkan/descriptorSets.h>

namespace atta::vk
{
	class DescriptorSetManager
	{
		public:
			DescriptorSetManager(std::shared_ptr<Device> device, std::vector<DescriptorBinding> descriptorBindings, size_t maxSets);
			~DescriptorSetManager();

			std::shared_ptr<Device> getDevice() const { return _device; }
			std::shared_ptr<DescriptorPool> getDescriptorPool() const { return _descriptorPool; }
			std::shared_ptr<DescriptorSetLayout> getDescriptorSetLayout() const { return _descriptorSetLayout; }
			std::shared_ptr<DescriptorSets> getDescriptorSets() const { return _descriptorSets; }

		private:
			std::shared_ptr<Device> _device;
			std::shared_ptr<DescriptorPool> _descriptorPool;
			std::shared_ptr<DescriptorSetLayout> _descriptorSetLayout;
			std::shared_ptr<DescriptorSets> _descriptorSets;
	};
}

#endif// ATTA_GRAPHICS_VULKAN_DESCRIPTOR_SET_MANAGER_H
