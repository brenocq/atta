//--------------------------------------------------
// Robot Simulator
// imageView.h
// Date: 24/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <set>
#include <string.h>
#include "defines.h"
#include "device.h"

class ImageView
{
	public:
	ImageView(Device* device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels=1);
	~ImageView();

	VkImageView handle() const { return _imageView; }
	Device* getDevice() const { return _device; }

	private:
    VkImageView _imageView;
	Device* _device;
	uint32_t _mipLevels;
};

#endif// IMAGE_VIEW_H
