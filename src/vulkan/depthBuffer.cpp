#include "depthBuffer.h"

DepthBuffer::DepthBuffer(CommandPool* commandPool, VkExtent2D extent):
		_format(findDepthFormat(commandPool->device()))
{
	const auto& device = commandPool->device();

	_image = new Image(device, extent, _format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
	_imageMemory = new DeviceMemory(_image->allocateMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
	_imageView = new ImageView(device, _image->handle(), _format, VK_IMAGE_ASPECT_DEPTH_BIT);

	_image->transitionImageLayout(commandPool, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

DepthBuffer::~DepthBuffer()
{
	delete _imageView;
	delete _image;
	delete _imageMemory;
}

VkFormat DepthBuffer::findSupportedFormat(const Device* device, const std::vector<VkFormat>& candidates, 
		const VkImageTiling tiling, const VkFormatFeatureFlags features)
{
	for (auto format : candidates)
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(device->physicalDevice(), format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
		{
			return format;
		}

		if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
		{
			return format;
		}
	}

	std::cout << BOLDRED << "[DepthBuffer] Failed to find supported format!" << RESET << std::endl;
	exit(1);
}

VkFormat DepthBuffer::findDepthFormat(const Device* device)
{
	return findSupportedFormat(
		device,
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}
