//--------------------------------------------------
// Robot Simulator
// descriptorSetManager.cpp
// Date: 23/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "descriptorSetManager.h"
#include "simulator/helpers/log.h"

DescriptorSetManager::DescriptorSetManager(Device* device, std::vector<DescriptorBinding> descriptorBindings, size_t maxSets)
{
	_device = device;
	
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

	_descriptorPool = new DescriptorPool(_device, descriptorBindings, maxSets);
	_descriptorSetLayout = new DescriptorSetLayout(_device, descriptorBindings);
	_descriptorSets = new DescriptorSets(_device, _descriptorPool, _descriptorSetLayout, bindingTypes, maxSets);
}

DescriptorSetManager::~DescriptorSetManager()
{
	delete _descriptorSets;
	_descriptorSets = nullptr;

	delete _descriptorSetLayout;
	_descriptorSetLayout = nullptr;

	delete _descriptorPool;
	_descriptorPool = nullptr;
}
