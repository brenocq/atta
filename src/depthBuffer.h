#ifndef DEPTH_BUFFER_H
#define DEPTH_BUFFER_H

#include "instance.h"
#include "commandPool.h"
#include "image.h"
#include "deviceMemory.h"
#include "imageView.h"

class DepthBuffer
{
	public:
	DepthBuffer(CommandPool* commandPool, VkExtent2D extent);
	~DepthBuffer();

	VkFormat format() const { return _format; }
	ImageView* imageView() const { return _imageView; }

	bool hasStencilComponent(const VkFormat format)
	{
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	private:
	VkFormat findSupportedFormat(const Device* device, const std::vector<VkFormat>& candidates, const VkImageTiling tiling, const VkFormatFeatureFlags features);
	VkFormat findDepthFormat(const Device* device);

	const VkFormat _format;
	Image* _image;
	DeviceMemory* _imageMemory;
	ImageView* _imageView;
};

#endif// DEPTH_BUFFER_H
