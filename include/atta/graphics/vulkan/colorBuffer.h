//--------------------------------------------------
// Robot Simulator
// colorBuffer.h
// Date: 2020-07-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VULKAN_COLOR_BUFFER_H
#define ATTA_GRAPHICS_VULKAN_COLOR_BUFFER_H

#include <iostream>
#include <string.h>
#include <atta/graphics/vulkan/device.h>
#include <atta/graphics/vulkan/swapChain.h>
#include <atta/graphics/vulkan/image.h>
#include <atta/graphics/vulkan/imageView.h>

namespace atta::vk
{
	class ColorBuffer
	{
		public:
			ColorBuffer(std::shared_ptr<Device> device, VkExtent2D extent, VkFormat format);
			~ColorBuffer();

			std::shared_ptr<Device> getDevice() const { return _device; }
			Image* getImage() const { return _image; }
			ImageView* getImageView() const { return _imageView; }
			VkExtent2D getExtent() const { return _extent; }
			VkFormat getFormat() const { return _format; }

		private:
			std::shared_ptr<Device> _device;
			Image* _image;
			ImageView* _imageView;
			VkExtent2D _extent;
			VkFormat _format;
	};
}

#endif// ATTA_GRAPHICS_VULKAN_COLOR_BUFFER_H
