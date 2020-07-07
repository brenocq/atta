//--------------------------------------------------
// Robot Simulator
// descriptorPool.cpp
// Date: 07/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "descriptorPool.h"

DescriptorPool::DescriptorPool(Device* device, const int size)
{
	_device = device;

	VkDescriptorPoolSize poolSize{};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = static_cast<uint32_t>(size);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = static_cast<uint32_t>(size);

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
