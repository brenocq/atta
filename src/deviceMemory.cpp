#include "deviceMemory.h"

DeviceMemory::DeviceMemory(
	const Device* device, 
	const size_t size, 
	const uint32_t memoryTypeBits, 
	const VkMemoryPropertyFlags properties) :
	_device(device)
{
	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = size;
	allocInfo.memoryTypeIndex = findMemoryType(memoryTypeBits, properties);

	if(vkAllocateMemory(device->handle(), &allocInfo, nullptr, &_memory) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[DeviceMemory] Failed to allocate memory!" << RESET << std::endl;
		exit(1);
	}
}

DeviceMemory::DeviceMemory(DeviceMemory&& other) noexcept :
	_device(other._device),
	_memory(other._memory)
{
	other._memory = nullptr;
}

DeviceMemory::~DeviceMemory()
{
	if (_memory != nullptr)
		vkFreeMemory(_device->handle(), _memory, nullptr);
}

void* DeviceMemory::mapMemory(const size_t offset, const size_t size)
{
	void* data;
	if(vkMapMemory(_device->handle(), _memory, offset, size, 0, &data) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[DeviceMemory] Failed to map memory!" << RESET << std::endl;
		exit(1);
	}

	return data;
}

void DeviceMemory::unmapMemory()
{
	vkUnmapMemory(_device->handle(), _memory);
}

uint32_t DeviceMemory::FindMemoryType(const uint32_t typeFilter, const VkMemoryPropertyFlags properties) const
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(_device->physicalDevice(), &memProperties);

	for (uint32_t i = 0; i != memProperties.memoryTypeCount; ++i)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	std::cout << BOLDRED << "[DeviceMemory] Failed to find suitable memory type!" << RESET << std::endl;
	exit(1);
}
