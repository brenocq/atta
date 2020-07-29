//--------------------------------------------------
// Robot Simulator
// commandBuffers.cpp
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "commandBuffers.h"
#include "physicalDevice.h"

CommandBuffers::CommandBuffers(Device* device, CommandPool* commandPool, uint32_t size)
{
	_device = device;
	_commandPool = commandPool;
	_commandBuffers.resize(size);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool->handle();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t) _commandBuffers.size();

	if(vkAllocateCommandBuffers(_device->handle(), &allocInfo, _commandBuffers.data()) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[CommandBuffers]" << RESET << RED << " Failed to create command buffers!" << RESET << std::endl;
		exit(1);
	}
}

CommandBuffers::~CommandBuffers()
{
	vkFreeCommandBuffers(_device->handle(), _commandPool->handle(), static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());
}

VkCommandBuffer CommandBuffers::begin(const size_t i)
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	beginInfo.pInheritanceInfo = nullptr; // Optional

	if(vkBeginCommandBuffer(_commandBuffers[i], &beginInfo) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[CommandBuffers]" << RESET << RED << " Failed to begin recording command buffer!" << RESET << std::endl;
		exit(1);
	}

	return _commandBuffers[i];
}

void CommandBuffers::end(const size_t i)
{

	if(vkEndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[CommandBuffers]" << RESET << RED << " Failed to record command buffer!" << RESET << std::endl;
		exit(1);
	}
}
