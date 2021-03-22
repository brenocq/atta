//--------------------------------------------------
// Robot Simulator
// depthBuffer.cpp
// Date: 2020-07-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/vulkan/depthBuffer.h>
#include <atta/graphics/vulkan/physicalDevice.h>
#include <atta/helpers/log.h>

namespace atta::vk
{
	DepthBuffer::DepthBuffer(std::shared_ptr<Device> device, VkExtent2D extent):
		_device(device), _extent(extent)
	{
		_format = findSupportedFormat(
			{VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D16_UNORM},
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);

		_image = new Image(_device, _extent.width, _extent.height, _format
				, VK_IMAGE_TILING_OPTIMAL
				, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
				, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				1, _device->getMsaaSamples());
		_imageView = new ImageView(_device, _image->handle(), _format, VK_IMAGE_ASPECT_DEPTH_BIT | (_stencilAvailable?VK_IMAGE_ASPECT_STENCIL_BIT:0));
	}

	DepthBuffer::~DepthBuffer()
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

	VkFormat DepthBuffer::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		_stencilAvailable = true;
		for (VkFormat format : candidates) 
		{
			VkFormatProperties props;
			std::shared_ptr<PhysicalDevice> physicalDevice = _device->getPhysicalDevice();
			vkGetPhysicalDeviceFormatProperties(physicalDevice->handle(), format, &props);

			if(format == VK_FORMAT_D32_SFLOAT || format == VK_FORMAT_D16_UNORM)
				_stencilAvailable = false;

			if(tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
			{
				return format;
			}
			else if(tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
			{
				return format;
			}
		}

		Log::error("DepthBuffer", "Failed to find supported format!");
		exit(1);
	}
}
