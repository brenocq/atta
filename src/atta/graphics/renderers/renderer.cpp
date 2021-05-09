//--------------------------------------------------
// Atta Graphics
// renderer.cpp
// Date: 2021-01-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/renderers/renderer.h>

namespace atta
{
	Renderer::Renderer(CreateInfo info):
		_vkCore(info.vkCore), _commandPool(info.commandPool), _extent({info.width, info.height}), _viewMatrix(info.viewMatrix), _type(info.type),
		_imageUsageFlags(info.imageUsageFlags), _forceNoMultisampling(info.forceNoMultisampling)
	{
		createOutputImage();
	}

	void Renderer::createOutputImage()
	{
		_image = std::make_shared<vk::Image>(
			_vkCore->getDevice(), 
			_extent.width, _extent.height, 
			VK_FORMAT_B8G8R8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT|VK_IMAGE_USAGE_TRANSFER_SRC_BIT|VK_IMAGE_USAGE_TRANSFER_DST_BIT|_imageUsageFlags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 1, 
			VK_SAMPLE_COUNT_1_BIT);
		_imageView = std::make_shared<vk::ImageView>(_vkCore->getDevice(), _image->handle(), VK_FORMAT_B8G8R8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);
	}
}
