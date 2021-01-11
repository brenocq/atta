//--------------------------------------------------
// Robot Simulator
// descriptorSetManager.h
// Date: 2020-07-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_DESCRIPTOR_SET_MANAGER_H
#define ATTA_VK_DESCRIPTOR_SET_MANAGER_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"
#include "descriptorBinding.h"
#include "descriptorPool.h"
#include "descriptorSetLayout.h"
#include "descriptorSets.h"

namespace atta::vk
{
	class DescriptorSetManager
	{
		public:
			DescriptorSetManager(std::shared_ptr<Device> device, std::vector<DescriptorBinding> descriptorBindings, size_t maxSets);
			~DescriptorSetManager();

			std::shared_ptr<Device> getDevice() const { return _device; }
			DescriptorPool* getDescriptorPool() const { return _descriptorPool; }
			DescriptorSetLayout* getDescriptorSetLayout() const { return _descriptorSetLayout; }
			DescriptorSets* getDescriptorSets() const { return _descriptorSets; }

		private:
			std::shared_ptr<Device> _device;
			DescriptorPool* _descriptorPool;
			DescriptorSetLayout* _descriptorSetLayout;
			DescriptorSets* _descriptorSets;
	};
}

#endif// ATTA_VK_DESCRIPTOR_SET_MANAGER_H
