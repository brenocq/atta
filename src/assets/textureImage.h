#ifndef TEXTURE_IMAGE_H
#define TEXTURE_IMAGE_H

#include <memory>
#include "texture.h"
#include "../vulkan/commandPool.h"
#include "../vulkan/deviceMemory.h"
#include "../vulkan/image.h"
#include "../vulkan/imageView.h"
#include "../vulkan/sampler.h"

class TextureImage
{
public:

	TextureImage(const TextureImage&) = delete;
	TextureImage(TextureImage&&) = delete;
	TextureImage& operator = (const TextureImage&) = delete;
	TextureImage& operator = (TextureImage&&) = delete;

	TextureImage(CommandPool* commandPool, const Texture& texture);
	~TextureImage();

	ImageView* imageView() const { return _imageView; }
	Sampler* sampler() const { return _sampler; }

private:

	Image* _image;
	DeviceMemory* _imageMemory;
	ImageView* _imageView;
	Sampler* _sampler;
};

#endif// TEXTURE_IMAGE_H
