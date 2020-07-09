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

	std::array<VkDescriptorPoolSize, 2> poolSizes{};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(size);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(size);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
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
