//--------------------------------------------------
// Robot Simulator
// imageView.cpp
// Date: 24/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "imageView.h"

ImageView::ImageView(Device* device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
	_device = device;

	VkImageViewCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image = image;
	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.format = format;
	createInfo.subresourceRange.aspectMask = aspectFlags;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

	if(vkCreateImageView(_device->handle(), &createInfo, nullptr, &_imageView) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[ImageView]" << RESET << RED << " Failed to create image view!" << RESET << std::endl;
		exit(1);
	}
}

ImageView::~ImageView()
{
	if(_imageView != nullptr)
	{
		vkDestroyImageView(_device->handle(), _imageView, nullptr);
		_imageView = nullptr;

	}
}
