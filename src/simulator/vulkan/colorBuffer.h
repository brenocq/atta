//--------------------------------------------------
// Robot Simulator
// colorBuffer.h
// Date: 09/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef COLOR_BUFFER_H
#define COLOR_BUFFER_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"
#include "swapChain.h"
#include "image.h"
#include "imageView.h"

class ColorBuffer
{
	public:
		ColorBuffer(Device* device, VkExtent2D extent, VkFormat format);
		~ColorBuffer();

		Device* getDevice() const { return _device; }
		Image* getImage() const { return _image; }
		ImageView* getImageView() const { return _imageView; }
		VkExtent2D getExtent() const { return _extent; }
		VkFormat getFormat() const { return _format; }

	private:
		Device* _device;
		Image* _image;
		ImageView* _imageView;
		VkExtent2D _extent;
		VkFormat _format;
};

#endif// COLOR_BUFFER_H
