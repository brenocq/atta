//--------------------------------------------------
// Robot Simulator
// texture.h
// Date: 08/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>
#include "defines.h"
#include "device.h"
#include "commandPool.h"
#include "image.h"
#include "imageView.h"
#include "sampler.h"

class Texture
{
	public:
	Texture(Device* device, CommandPool* commandPool, std::string filename);
	~Texture();

	Device* getDevice() const { return _device; }
	Image* getImage() const { return _image; }
	ImageView* getImageView() const { return _imageView; }
	Sampler* getSampler() const { return _sampler; }

	private:
	void transitionImageLayout(VkFormat format, VkImageLayout newLayout);
	void copyBufferToImage(VkBuffer buffer, uint32_t width, uint32_t height);
	void generateMipmaps();
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	Device* _device;
	CommandPool* _commandPool;
	Image* _image;
	ImageView* _imageView;
	Sampler* _sampler;
	uint32_t _mipLevels;
	int32_t _width, _height;
};

#endif// TEXTURE_H
