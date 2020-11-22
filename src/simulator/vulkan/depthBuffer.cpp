//--------------------------------------------------
// Robot Simulator
// depthBuffer.cpp
// Date: 08/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "depthBuffer.h"
#include "physicalDevice.h"

DepthBuffer::DepthBuffer(Device* device, VkExtent2D extent):
	_extent(extent)
{
	_device = device;
	_format = findSupportedFormat(
        {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );

	_image = new Image(_device, _extent.width, _extent.height, _format
			, VK_IMAGE_TILING_OPTIMAL
			, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			1, _device->getMsaaSamples());
	_imageView = new ImageView(_device, _image->handle(), _format, VK_IMAGE_ASPECT_DEPTH_BIT);
	//transitionImageLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
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
	for (VkFormat format : candidates) 
	{
		VkFormatProperties props;
		PhysicalDevice* physicalDevice = _device->getPhysicalDevice();
		vkGetPhysicalDeviceFormatProperties(physicalDevice->handle(), format, &props);

		if(tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
		{
			return format;
		}
		else if(tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
		{
			return format;
		}
	}

	std::cout << BOLDRED << "[DepthBuffer]" << RESET << RED << " Failed to find supported format!" << RESET << std::endl;
	exit(1);
}
