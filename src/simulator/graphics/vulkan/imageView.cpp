//--------------------------------------------------
// Robot Simulator
// imageView.cpp
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "imageView.h"

namespace atta::vk
{
	ImageView::ImageView(std::shared_ptr<Device> device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels, bool isCubeMap):
		_device(device), _mipLevels(mipLevels)
	{
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = image;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = format;
		createInfo.subresourceRange.aspectMask = aspectFlags;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = _mipLevels;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_A;

		if(isCubeMap)
		{
			createInfo.viewType =  VK_IMAGE_VIEW_TYPE_CUBE;
			createInfo.subresourceRange.layerCount = 6;
		}

		if(vkCreateImageView(_device->handle(), &createInfo, nullptr, &_imageView) != VK_SUCCESS)
		{
			std::cout << BOLDRED << "[ImageView]" << RESET << RED << " Failed to create image view!" << RESET << std::endl;
			exit(1);
		}
	}

	ImageView::~ImageView()
	{
		vkDestroyImageView(_device->handle(), _imageView, nullptr);
	}
}
