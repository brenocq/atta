#include "descriptorSetManager.h"
#include <set>

DescriptorSetManager::DescriptorSetManager(Device* device, const std::vector<DescriptorBinding>& descriptorBindings, const size_t maxSets)
{
	// Sanity check to avoid binding different resources to the same binding point.
	std::map<uint32_t, VkDescriptorType> bindingTypes;

	for (const auto& binding : descriptorBindings)
	{
		if (!bindingTypes.insert(std::make_pair(binding.Binding, binding.Type)).second)
		{
			std::cout << BOLDRED << "[DescriptorSetManager] Binding collision!" << RESET << std::endl;
			exit(1);
		}
	}

	_descriptorPool = new DescriptorPool(device, descriptorBindings, maxSets);
	_descriptorSetLayout = new DescriptorSetLayout(device, descriptorBindings);
	_descriptorSets = new DescriptorSets(_descriptorPool, _descriptorSetLayout, bindingTypes, maxSets);
}

DescriptorSetManager::~DescriptorSetManager()
{
	delete _descriptorSets;
	delete _descriptorSetLayout;
	delete _descriptorPool;
}
