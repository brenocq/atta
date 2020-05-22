#include "descriptorPool.h"

DescriptorPool::DescriptorPool(Device* device, const std::vector<DescriptorBinding>& descriptorBindings, const size_t maxSets) :
	_device(device)
{
	std::vector<VkDescriptorPoolSize> poolSizes;

	for (const auto& binding : descriptorBindings)
	{
		poolSizes.push_back(VkDescriptorPoolSize{ binding.Type, static_cast<uint32_t>(binding.DescriptorCount*maxSets )});
	}

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(maxSets);

	if(vkCreateDescriptorPool(device->handle(), &poolInfo, nullptr, &_descriptorPool) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[DescriptorPool] Failed to create descriptor pool!" << RESET << std::endl;
		exit(1);
	}
}

DescriptorPool::~DescriptorPool()
{
	if (_descriptorPool != nullptr)
		vkDestroyDescriptorPool(_device->handle(), _descriptorPool, nullptr);
}
