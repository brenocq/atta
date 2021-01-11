//--------------------------------------------------
// Robot Simulator
// commandPool.cpp
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "commandPool.h"
#include "physicalDevice.h"
#include "simulator/helpers/log.h"

namespace atta::vk
{
	CommandPool::CommandPool(std::shared_ptr<Device> device, VkCommandPoolCreateFlags flags):
		_device(device)
	{
		std::shared_ptr<PhysicalDevice> physicalDevice = _device->getPhysicalDevice();

		QueueFamilyIndices queueFamilyIndices = physicalDevice->findQueueFamilies();

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
		poolInfo.flags = flags;

		if(vkCreateCommandPool(_device->handle(), &poolInfo, nullptr, &_commandPool) != VK_SUCCESS)
		{
			Log::error("CommandPool", "Failed to create command pool!");
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

	VkCommandBuffer CommandPool::beginSingleTimeCommands()
	{
		// Allocation is slow... maybe use some command buffer already in the command pool
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = _commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(_device->handle(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void CommandPool::endSingleTimeCommands(VkCommandBuffer commandBuffer) 
	{
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		VkQueue graphicsQueue = _device->getGraphicsQueue();

		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsQueue);

		vkFreeCommandBuffers(_device->handle(), _commandPool, 1, &commandBuffer);
	}
}
