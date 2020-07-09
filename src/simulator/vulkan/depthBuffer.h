//--------------------------------------------------
// Robot Simulator
// depthBuffer.h
// Date: 08/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef DEPTH_BUFFER_H
#define DEPTH_BUFFER_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"
#include "commandPool.h"
#include "image.h"
#include "imageView.h"

class DepthBuffer
{
	public:
	DepthBuffer(Device* device, CommandPool* commandPool, VkExtent2D extent);
	~DepthBuffer();

	Device* getDevice() const { return _device; }
	VkFormat getFormat() const { return _format; }
	Image* getImage() const { return _image; }
	ImageView* getImageView() const { return _imageView; }

	private:
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	bool hasStencilComponent();

	// TODO also being used in Texture
	void transitionImageLayout(VkImageLayout newLayout);
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	Device* _device;
	CommandPool* _commandPool;
	Image* _image;
	ImageView* _imageView;
	VkFormat _format;
	VkExtent2D _extent;
};

#endif// DEPTH_BUFFER_H
