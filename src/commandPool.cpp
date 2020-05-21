#include "commandPool.h"

CommandPool::CommandPool(Device* device, uint32_t queueFamilyIndex, bool allowReset):
	_device(device)
{
	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndex;
	poolInfo.flags = allowReset ? VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT : 0;

	if(vkCreateCommandPool(_device->handle(), &poolInfo, nullptr, &_commandPool) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[CommandPool] Failed to create command pool!" << RESET << std::endl;
		exit(1);
	}
}

CommandPool::~CommandPool()
{
	if (_commandPool != nullptr)
		vkDestroyCommandPool(_device->handle(), _commandPool, nullptr);
}
