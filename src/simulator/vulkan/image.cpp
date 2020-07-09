//--------------------------------------------------
// Robot Simulator
// image.cpp
// Date: 07/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "image.h"

Image::Image(Device* device, 
		uint32_t width, uint32_t height, 
		VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, uint32_t mipLevels, VkSampleCountFlagBits numSamples):
	_format(format), _layout(VK_IMAGE_LAYOUT_UNDEFINED), _mipLevels(mipLevels)
{
	_device = device;

	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = _mipLevels;
	imageInfo.arrayLayers = 1;

	imageInfo.format = format;
	// Use VK_IMAGE_TILING_LINEAR if wants to access the image data
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = numSamples;
	imageInfo.flags = 0; // Optional

	if (vkCreateImage(_device->handle(), &imageInfo, nullptr, &_image) != VK_SUCCESS) {
		std::cout << BOLDRED << "[Image]" << RESET << RED << " Failed to create image!" << RESET << std::endl;
		exit(1);
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(_device->handle(), _image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(_device->handle(), &allocInfo, nullptr, &_memory) != VK_SUCCESS) {
		std::cout << BOLDRED << "[Image]" << RESET << RED << " Failed to allocate image memory!" << RESET << std::endl;
		exit(1);
	}

	vkBindImageMemory(_device->handle(), _image, _memory, 0);
}

Image::~Image()
{
	if (_image != nullptr)
	{
		vkDestroyImage(_device->handle(), _image, nullptr);
		_image = nullptr;
		vkFreeMemory(_device->handle(), _memory, nullptr);
	}
}

uint32_t Image::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) 
{
	// Copied from buffer class. TODO get from same place
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

	std::cout << BOLDRED << "[Image]" << RESET << RED << " Failed to find suitable memory type!" << RESET << std::endl;
	exit(1);
}
