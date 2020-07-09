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
	ColorBuffer(Device* device, SwapChain* swapChain, VkExtent2D extent);
	~ColorBuffer();

	Device* getDevice() const { return _device; }
	Image* getImage() const { return _image; }
	ImageView* getImageView() const { return _imageView; }

	private:

	Device* _device;
	SwapChain* _swapChain;
	Image* _image;
	ImageView* _imageView;
	VkExtent2D _extent;
};

#endif// COLOR_BUFFER_H
