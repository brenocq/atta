//--------------------------------------------------
// Robot Simulator
// colorBuffer.cpp
// Date: 2020-07-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "colorBuffer.h"

namespace atta::vk
{
	ColorBuffer::ColorBuffer(std::shared_ptr<Device> device, VkExtent2D extent, VkFormat format):
		_device(device), _extent(extent), _format(format)
	{
		_image = new Image(_device, _extent.width, _extent.height, _format
				, VK_IMAGE_TILING_OPTIMAL
				, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
				, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
				, 1, device->getMsaaSamples());
		_imageView = new ImageView(_device, _image->handle(), _format, VK_IMAGE_ASPECT_COLOR_BIT);
	}

	ColorBuffer::~ColorBuffer()
	{
		if(_image != nullptr)
		{
			delete _image;
			_image = nullptr;
		}

		if(_imageView != nullptr)
		{
			delete _imageView;
			_imageView = nullptr;
		}
	}
}
