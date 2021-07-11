//--------------------------------------------------
// Robot Simulator
// texture.h
// Date: 2020-07-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VULKAN_TEXTURE_H
#define ATTA_GRAPHICS_VULKAN_TEXTURE_H

#include <iostream>
#include <string>
#include <mutex>
#include <atta/graphics/vulkan/device.h>
#include <atta/graphics/vulkan/commandPool.h>
#include <atta/graphics/vulkan/image.h>
#include <atta/graphics/vulkan/imageView.h>
#include <atta/graphics/vulkan/sampler.h>
#include <atta/graphics/core/texture.h>

namespace atta::vk
{
	class Texture
	{
		public:
			Texture(std::shared_ptr<Device> device, std::shared_ptr<CommandPool> commandPool, std::string filename, VkFormat format=VK_FORMAT_R8G8B8A8_SRGB);
			Texture(std::shared_ptr<Device> device, 
					std::shared_ptr<CommandPool> commandPool, 
					VkExtent2D size, 
					VkFormat format = VK_FORMAT_R8G8B8A8_SRGB, 
					VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
			Texture(std::shared_ptr<Device> device, std::shared_ptr<CommandPool> commandPool, void* buffer, VkExtent2D size, atta::Texture::Format format, bool editable=true);
			~Texture();

			std::shared_ptr<Device> getDevice() const { return _device; }
			std::shared_ptr<Image> getImage() const { return _image; }
			std::shared_ptr<ImageView> getImageView() const { return _imageView; }
			std::shared_ptr<Sampler> getSampler() const { return _sampler; }

			void updateImage(void* data);
			void lock() { if(_editable) _mtx.lock(); }
			void unlock() { if(_editable) _mtx.unlock(); }

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
			size_t _size;

			// Texture syncronization (needed when GUI thread tries to draw the texture on the screen and the thread manager wants to edit it)
			bool _editable;
			std::mutex _mtx;
	};
}

#endif// ATTA_GRAPHICS_VULKAN_TEXTURE_H
