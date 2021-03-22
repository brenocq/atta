//--------------------------------------------------
// Robot Simulator
// descriptorSetManager.cpp
// Date: 2020-07-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/vulkan/descriptorSetManager.h>
#include <atta/helpers/log.h>

namespace atta::vk
{
	DescriptorSetManager::DescriptorSetManager(
			std::shared_ptr<Device> device, 
			std::vector<DescriptorBinding> descriptorBindings, 
			size_t maxSets):
		_device(device)
	{
		
		// Sanity check to avoid binding different resources to the same binding point
		std::map<uint32_t, VkDescriptorType> bindingTypes;

		for(const auto& binding : descriptorBindings)
		{
			if(!bindingTypes.insert(std::make_pair(binding.binding, binding.type)).second)
			{
				Log::warning("DescriptorSetManager", "Binding collision detected!");
				return;
			}
		}

		_descriptorPool = std::make_shared<DescriptorPool>(_device, descriptorBindings, maxSets);
		_descriptorSetLayout = std::make_shared<DescriptorSetLayout>(_device, descriptorBindings);
		_descriptorSets = std::make_shared<DescriptorSets>(_device, _descriptorPool, _descriptorSetLayout, bindingTypes, maxSets);
	}

	DescriptorSetManager::~DescriptorSetManager()
	{
	}
}
