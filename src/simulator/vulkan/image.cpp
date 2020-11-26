//--------------------------------------------------
// Robot Simulator
// image.cpp
// Date: 07/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "image.h"
#include "simulator/helpers/log.h"
#include "simulator/vulkan/imageMemoryBarrier.h"

Image::Image(Device* device, 
		uint32_t width, uint32_t height, 
		VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, uint32_t mipLevels, VkSampleCountFlagBits numSamples):
	_format(format), _extent({width, height}), _layout(VK_IMAGE_LAYOUT_UNDEFINED), _mipLevels(mipLevels)
{
	_device = device;

	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = _extent.width;
	imageInfo.extent.height = _extent.height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = _mipLevels;
	imageInfo.arrayLayers = 1;

	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = numSamples;
	imageInfo.flags = 0; // Optional

	if(vkCreateImage(_device->handle(), &imageInfo, nullptr, &_image) != VK_SUCCESS)
	{
		Log::error("Image", "Failed to create image!");
		exit(1);
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(_device->handle(), _image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

	if(vkAllocateMemory(_device->handle(), &allocInfo, nullptr, &_memory) != VK_SUCCESS)
	{
		Log::error("Image", "Failed to allocate image memory!");
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

std::vector<uint8_t> Image::getBuffer(CommandPool* commandPool)
{
	Image* dstImg = new Image(_device, _extent.width, _extent.height,
			_format, VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	VkCommandBuffer commandBuffer = commandPool->beginSingleTimeCommands();
	{
		VkImageSubresourceRange subresourceRange;
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.baseArrayLayer = 0;
		subresourceRange.layerCount = 1;

		// Change transfer layouts
		ImageMemoryBarrier::insert(commandBuffer, _image, subresourceRange, 
			VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT, _layout, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
		ImageMemoryBarrier::insert(commandBuffer, dstImg->handle(), subresourceRange, 
			0, VK_ACCESS_TRANSFER_WRITE_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		// Copy image
		VkImageCopy imageCopyRegion{};
		imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageCopyRegion.srcSubresource.layerCount = 1;
		imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageCopyRegion.dstSubresource.layerCount = 1;
		imageCopyRegion.extent.width = _extent.width;
		imageCopyRegion.extent.height = _extent.height;
		imageCopyRegion.extent.depth = 1;

		// Issue the copy command
		vkCmdCopyImage(
			commandBuffer,
			_image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			dstImg->handle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&imageCopyRegion);

		// Change layouts back
		ImageMemoryBarrier::insert(commandBuffer, _image, subresourceRange, 
			VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);
		ImageMemoryBarrier::insert(commandBuffer, dstImg->handle(), subresourceRange, 
			0, VK_ACCESS_TRANSFER_WRITE_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);
	}
	commandPool->endSingleTimeCommands(commandBuffer);

	// Get image layout (including row pitch)
	VkSubresourceLayout subResourceLayout;
	VkImageSubresource subResource { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0 };
	vkGetImageSubresourceLayout(_device->handle(), dstImg->handle(), &subResource, &subResourceLayout);
	//Log::debug("Image", std::string("offset ")+std::to_string(subResourceLayout.offset) + 
	//		std::string(" size ")+std::to_string(subResourceLayout.size) + 
	//		std::string(" rowpitch ")+std::to_string(subResourceLayout.rowPitch) + 
	//		std::string(" arraypitch ")+std::to_string(subResourceLayout.arrayPitch));

	char* data;
	vkMapMemory(_device->handle(), dstImg->getMemory(), 0, VK_WHOLE_SIZE, 0, (void**)&data);
	data += subResourceLayout.offset;

	delete dstImg;
	dstImg = nullptr;

	std::vector<uint8_t> buffer(_extent.width*_extent.height*3);
	for(unsigned int y=0; y<_extent.height; y++)
	{
		for(unsigned int x=0; x<_extent.width; x++)
		{
			buffer[y*_extent.width*3 + x*3 + 2]	= data[y*(_extent.width*4) + x*4 + 0];
			buffer[y*_extent.width*3 + x*3 + 1]	= data[y*(_extent.width*4) + x*4 + 1];
			buffer[y*_extent.width*3 + x*3 + 0]	= data[y*(_extent.width*4) + x*4 + 2];
		}
	}
	return buffer;
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

	Log::error("Image", "Failed to find suitable memory type!");
	exit(1);
}
