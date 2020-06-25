//--------------------------------------------------
// Robot Simulator
// commandPool.cpp
// Date: 24/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "commandPool.h"
#include "physicalDevice.h"

CommandPool::CommandPool(Device* device)
{
	_device = device;
	PhysicalDevice* physicalDevice = _device->getPhysicalDevice();

	QueueFamilyIndices queueFamilyIndices = physicalDevice->findQueueFamilies();

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
	poolInfo.flags = 0; // Optional

	if(vkCreateCommandPool(_device->handle(), &poolInfo, nullptr, &_commandPool) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[CommandPool]" << RESET << RED << " Failed to create command pool!" << RESET << std::endl;
		exit(1);
	}
}

CommandPool::~CommandPool()
{

	if(_commandPool != nullptr)
	{
		vkDestroyCommandPool(_device->handle(), _commandPool, nullptr);
		_commandPool = nullptr;

	}
}
