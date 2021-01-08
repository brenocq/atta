//--------------------------------------------------
// Robot Simulator
// descriptorPool.cpp
// Date: 2020-07-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "descriptorPool.h"
#include "simulator/helpers/log.h"

DescriptorPool::DescriptorPool(std::shared_ptr<Device> device, std::vector<DescriptorBinding> descriptorBindings, size_t maxSets):
	_device(device)
{

	std::vector<VkDescriptorPoolSize> poolSizes;
	for(auto binding : descriptorBindings)
	{
		poolSizes.push_back(VkDescriptorPoolSize{ binding.type, static_cast<uint32_t>(binding.descriptorCount*maxSets )});
	}

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(maxSets);

	if(vkCreateDescriptorPool(_device->handle(), &poolInfo, nullptr, &_descriptorPool) != VK_SUCCESS)
	{
		Log::error("DescriptorPool", "Failed to create descriptor pool!");
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
