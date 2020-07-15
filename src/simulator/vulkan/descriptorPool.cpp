//--------------------------------------------------
// Robot Simulator
// descriptorPool.cpp
// Date: 07/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "descriptorPool.h"

DescriptorPool::DescriptorPool(Device* device, std::vector<VkDescriptorPoolSize> poolSizes)
{
	_device = device;

	int maxSets = 0;
	for(auto poolSize : poolSizes)
		if(poolSize.descriptorCount > maxSets)
			maxSets = poolSize.descriptorCount;

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(maxSets);

	if(vkCreateDescriptorPool(_device->handle(), &poolInfo, nullptr, &_descriptorPool) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[DescriptorPool]" << RESET << RED << " Failed to create descriptor pool!" << RESET << std::endl;
		exit(1);
	}
}

DescriptorPool::~DescriptorPool()
{
	if(_descriptorPool != nullptr)
	{
		vkDestroyDescriptorPool(_device->handle(), _descriptorPool, nullptr);
		_descriptorPool = nullptr;

	}
}
