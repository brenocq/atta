#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include "device.h"

class ImageView
{
	public:
	ImageView(Device* device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	~ImageView();

	VkImageView handle() const { return _imageView; }
	Device* device() const { return _device; }

	private:
	Device* _device;
	VkImageView _imageView;
	VkImage _image;
	VkFormat _format;
};

#endif// IMAGE_VIEW_H
