#include "buffer.h"
#include "commandBuffers.h"

Buffer::Buffer(Device* device, const size_t size, const VkBufferUsageFlags usage) :
	_device(device)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if(vkCreateBuffer(device->handle(), &bufferInfo, nullptr, &_buffer) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[Buffer] Failed to create buffer!" << RESET << std::endl;
		exit(1);
	}
}

Buffer::~Buffer()
{
	if (_buffer != nullptr)
		vkDestroyBuffer(_device->handle(), _buffer, nullptr);
}

DeviceMemory Buffer::allocateMemory(const VkMemoryPropertyFlags properties)
{
	const auto requirements = getMemoryRequirements();
	DeviceMemory memory(_device, requirements.size, requirements.memoryTypeBits, properties);

	if(vkBindBufferMemory(_device->handle(), _buffer, memory.handle(), 0) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[Buffer] Failed bind buffer memory!" << RESET << std::endl;
		exit(1);
	}

	return memory;
}

VkMemoryRequirements Buffer::getMemoryRequirements() const
{
	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(_device->handle(), _buffer, &requirements);
	return requirements;
}

void Buffer::copyFrom(CommandPool* commandPool, Buffer* src, VkDeviceSize size)
{
	CommandBuffers commandBuffers(commandPool, 1);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffers[0], &beginInfo);

		VkBufferCopy copyRegion = {};
		copyRegion.srcOffset = 0; // Optional
		copyRegion.dstOffset = 0; // Optional
		copyRegion.size = size;

		vkCmdCopyBuffer(commandBuffers[0], src->handle(), handle(), 1, &copyRegion);

	vkEndCommandBuffer(commandBuffers[0]);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[0];

	const auto graphicsQueue = commandPool->device()->graphicsQueue();

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, nullptr);
	vkQueueWaitIdle(graphicsQueue);
}
