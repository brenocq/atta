//--------------------------------------------------
// Robot Simulator
// buffer.cpp
// Date: 06/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "buffer.h"

Buffer::Buffer(Device* device, const int size, const VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
	_device = device;

	_bufferInfo = {};
    _bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    _bufferInfo.size = size;
    _bufferInfo.usage = usage;
    _bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if(vkCreateBuffer(_device->handle(), &_bufferInfo, nullptr, &_buffer) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[Buffer]" << RESET << RED << " Failed to create buffer!" << RESET << std::endl;
		exit(1);
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(_device->handle(), _buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

	if(vkAllocateMemory(_device->handle(), &allocInfo, nullptr, &_bufferMemory) != VK_SUCCESS) 
	{
		std::cout << BOLDRED << "[Buffer]" << RESET << RED << " Failed to allocate buffer memory!" << RESET << std::endl;
		exit(1);
	}
	vkBindBufferMemory(_device->handle(), _buffer, _bufferMemory, 0);
}

Buffer::~Buffer()
{
	if(_buffer != nullptr)
	{
		vkDestroyBuffer(_device->handle(), _buffer, nullptr);
		_buffer = nullptr;
	}
	if(_bufferMemory != nullptr)
	{
		vkFreeMemory(_device->handle(), _bufferMemory, nullptr);
		_bufferMemory = nullptr;
	}
}

uint32_t Buffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) 
{
	PhysicalDevice* physicalDevice = _device->getPhysicalDevice();
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice->handle(), &memProperties);

	for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
	{
		if((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	std::cout << BOLDRED << "[Buffer]" << RESET << RED << " Failed to find suitable memory type!" << RESET << std::endl;
	exit(1);
}

void Buffer::copyFrom(CommandPool* commandPool, VkBuffer srcBuffer, VkDeviceSize size)
{
	VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool->handle();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(_device->handle(), &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);
	{
		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0; // Optional
		copyRegion.dstOffset = 0; // Optional
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, _buffer, 1, &copyRegion);
	}
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	const auto graphicsQueue = _device->getGraphicsQueue();

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(_device->handle(), commandPool->handle(), 1, &commandBuffer);
}
