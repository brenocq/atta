//--------------------------------------------------
// Robot Simulator
// commandPool.cpp
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/vulkan/commandPool.h>
#include <atta/graphics/vulkan/physicalDevice.h>
#include <atta/helpers/log.h>

namespace atta::vk
{
	CommandPool::CommandPool(std::shared_ptr<Device> device, DeviceQueueFamily deviceQueueFamily, SubmitQueueType submitQueueType, VkCommandPoolCreateFlags flags):
		_device(device), _deviceQueueFamily(deviceQueueFamily), _submitQueueType(submitQueueType)
	{
		std::shared_ptr<PhysicalDevice> physicalDevice = _device->getPhysicalDevice();
		QueueFamilyIndices queueFamilyIndices = physicalDevice->getQueueFamilyIndices();

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		switch(_deviceQueueFamily)
		{
			case DEVICE_QUEUE_FAMILY_GRAPHICS:
				// Select queue family
				poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

				// Select queue from queue family
				switch(_submitQueueType)
				{
					case QUEUE_THREAD_MANAGER:
						_submitQueue = _device->getGraphicsQueue();
						break;
					case QUEUE_GUI:
						_submitQueue = _device->getGraphicsQueueGUI();
						break;
					default:
						Log::error("CommandPool", "Command pool(graphics) submit queue must be THREAD_MANAGER or GUI!");
						exit(1);
				}
				break;
			case DEVICE_QUEUE_FAMILY_TRANSFER:
				// Select queue family
				poolInfo.queueFamilyIndex = queueFamilyIndices.transferFamily.value();

				// Select queue from queue family
				switch(_submitQueueType)
				{
					case QUEUE_THREAD_MANAGER:
						_submitQueue = _device->getTransferQueue();
						break;
					case QUEUE_GUI:
						_submitQueue = _device->getTransferQueueGUI();
						break;
					default:
						Log::error("CommandPool", "Command pool(transfer) submit queue must be THREAD_MANAGER or GUI!");
						exit(1);
				}
				break;
			default:
				Log::error("CommandPool", "Command pool device queue family must be graphics or transfer!");
				exit(1);
		}
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

		vkQueueSubmit(_submitQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(_submitQueue);

		vkFreeCommandBuffers(_device->handle(), _commandPool, 1, &commandBuffer);
	}
}
