#include "commandBuffers.h"

CommandBuffers::CommandBuffers(CommandPool* commandPool, uint32_t size):
	_commandPool(commandPool)
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool->handle();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = size;

	_commandBuffers.resize(size);

	if(vkAllocateCommandBuffers(commandPool->device()->handle(), &allocInfo, _commandBuffers.data()) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[CommandBuffers] Failed to allocate command buffers!" << RESET << std::endl;
		exit(1);
	}
}

CommandBuffers::~CommandBuffers()
{
	if (!_commandBuffers.empty())
	{
		vkFreeCommandBuffers(_commandPool->device()->handle(), _commandPool->handle(), static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());
		_commandBuffers.clear();
	}
}

VkCommandBuffer CommandBuffers::begin(const size_t i)
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;//VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	beginInfo.pInheritanceInfo = nullptr; // Optional

	if(vkBeginCommandBuffer(_commandBuffers[i], &beginInfo) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[CommandBuffers] Failed to begin recording command buffer!" << RESET << std::endl;
		exit(1);
	}

	return _commandBuffers[i];
}

void CommandBuffers::end(const size_t i)
{

	if(vkEndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[CommandBuffers] Failed to record command buffer!" << RESET << std::endl;
		exit(1);
	}
}
