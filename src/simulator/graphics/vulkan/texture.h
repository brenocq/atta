//--------------------------------------------------
// Robot Simulator
// texture.h
// Date: 2020-07-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_TEXTURE_H
#define ATTA_VK_TEXTURE_H

#include <iostream>
#include <string>
#include "defines.h"
#include "device.h"
#include "commandPool.h"
#include "image.h"
#include "imageView.h"
#include "sampler.h"

namespace atta::vk
{
	class Texture
	{
		public:
			Texture(std::shared_ptr<Device> device, std::shared_ptr<CommandPool> commandPool, std::string filename, VkFormat format=VK_FORMAT_R8G8B8A8_SRGB);
			Texture(std::shared_ptr<Device> device, std::shared_ptr<CommandPool> commandPool, VkExtent2D size);
			Texture(std::shared_ptr<Device> device, std::shared_ptr<CommandPool> commandPool, unsigned char buffer[],  VkExtent2D size);
			~Texture();

			std::shared_ptr<Device> getDevice() const { return _device; }
			std::shared_ptr<Image> getImage() const { return _image; }
			std::shared_ptr<ImageView> getImageView() const { return _imageView; }
			std::shared_ptr<Sampler> getSampler() const { return _sampler; }

			void updateTextureImage(std::vector<uint8_t> pixels);

		private:
			void transitionImageLayout(VkImageLayout newLayout);
			void copyBufferToImage(VkBuffer buffer, uint32_t width, uint32_t height);
			void copyBufferToCubeMapImages(VkBuffer buffer, uint32_t width, uint32_t height);
			void copyEquToCubeMapImages(float* buffer, uint32_t width, uint32_t height);
			void generateMipmaps();

			std::shared_ptr<Device> _device;
			std::shared_ptr<CommandPool> _commandPool;
			std::shared_ptr<Image> _image;
			std::shared_ptr<ImageView> _imageView;
			std::shared_ptr<Sampler> _sampler;
			uint32_t _mipLevels;
			uint32_t _arrayLayers;
			int32_t _width, _height;
	};
}

#endif// ATTA_VK_TEXTURE_H
