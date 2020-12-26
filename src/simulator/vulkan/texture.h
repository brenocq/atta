//--------------------------------------------------
// Robot Simulator
// texture.h
// Date: 2020-07-08
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
		Texture(Device* device, CommandPool* commandPool, std::string filename, VkFormat format=VK_FORMAT_R8G8B8A8_SRGB);
		Texture(Device* device, CommandPool* commandPool, VkExtent2D size);
		Texture(Device* device, CommandPool* commandPool, unsigned char buffer[],  VkExtent2D size);
		~Texture();

		Device* getDevice() const { return _device; }
		Image* getImage() const { return _image; }
		ImageView* getImageView() const { return _imageView; }
		Sampler* getSampler() const { return _sampler; }

		void updateTextureImage(std::vector<uint8_t> pixels);

	private:
		void transitionImageLayout(VkImageLayout newLayout);
		void copyBufferToImage(VkBuffer buffer, uint32_t width, uint32_t height);
		void copyBufferToCubeMapImages(VkBuffer buffer, uint32_t width, uint32_t height);
		void copyEquToCubeMapImages(float* buffer, uint32_t width, uint32_t height);
		void generateMipmaps();

		Device* _device;
		CommandPool* _commandPool;
		Image* _image;
		ImageView* _imageView;
		Sampler* _sampler;
		uint32_t _mipLevels;
		uint32_t _arrayLayers;
		int32_t _width, _height;
};

#endif// TEXTURE_H
