//--------------------------------------------------
// Robot Simulator
// image.h
// Date: 07/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"

class Image
{
	public:
	Image(Device* device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
	~Image();

	Device* getDevice() const { return _device; }
	VkImage handle() const { return _image; }

	private:
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	Device* _device;
	VkImage _image;
	VkDeviceMemory _memory;
};

#endif// IMAGE_H
