//--------------------------------------------------
// Robot Simulator
// imageView.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_IMAGE_VIEW_H
#define ATTA_VK_IMAGE_VIEW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <set>
#include <string.h>
#include "defines.h"
#include "device.h"

namespace atta::vk
{
	class ImageView
	{
		public:
			ImageView(std::shared_ptr<Device> device, VkImage image, 
					VkFormat format, VkImageAspectFlags aspectFlags, 
					uint32_t mipLevels=1, bool isCubeMap=false);
			~ImageView();

			VkImageView handle() const { return _imageView; }
			std::shared_ptr<Device> getDevice() const { return _device; }
			VkFormat getFormat() const { return _format; }

		private:
			VkImageView _imageView;
			std::shared_ptr<Device> _device;
			uint32_t _mipLevels;
			VkFormat _format;
	};
}

#endif// ATTA_VK_IMAGE_VIEW_H
