//--------------------------------------------------
// Robot Simulator
// texture.cpp
// Date: 2020-07-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "texture.h"
#include "stagingBuffer.h"
#include "commandPool.h"
#include "stbImage.h"
#include "simulator/helpers/log.h"

namespace atta::vk
{
	Texture::Texture(std::shared_ptr<Device> device, std::shared_ptr<CommandPool> commandPool, std::string filename, VkFormat format):
		_device(device), _commandPool(commandPool), _arrayLayers(1)
	{
		int texWidth, texHeight, texChannels;
		void* pixels = nullptr;

		// Load image to memory
		if(format == VK_FORMAT_R32G32B32A32_SFLOAT)
		{
			stbi_set_flip_vertically_on_load(true);
			float* helper = stbi_loadf(filename.c_str(), &texWidth, &texHeight, &texChannels, 0);
			stbi_set_flip_vertically_on_load(false);
			pixels = new float[texWidth*texHeight*4];
			float* data = (float*)pixels;
			for(int x=0; x<texWidth; x++)
			{
				for(int y=0; y<texHeight; y++)
				{
					data[y*texWidth*4+x*4+1] = helper[y*texWidth*3+x*3+1];
					data[y*texWidth*4+x*4+0] = helper[y*texWidth*3+x*3+0];
					data[y*texWidth*4+x*4+2] = helper[y*texWidth*3+x*3+2];
					data[y*texWidth*4+x*4+3] = 1;
				}
			}
		}
		else
			pixels = stbi_load(filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

		// Check error while laoding
		if(!pixels) 
		{
			std::cout << BOLDRED << "[Texture]" << RESET << RED << " Failed to load texture image! (float format)" << RESET;
			std::cout << RED << " (Path: " << WHITE << filename << RED << ")";
			if(stbi_failure_reason())
				std::cout << RED  << " (Error: " << WHITE << stbi_failure_reason() << RED << ")";
			std::cout << RESET << std::endl;
			exit(1);
		}

		_mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
		_width = texWidth;
		_height = texHeight;

		// Create staging buffer to copy data
		StagingBuffer* stagingBuffer;
		VkDeviceSize imageSize;
		if(format == VK_FORMAT_R32G32B32A32_SFLOAT)
			imageSize = texWidth * texHeight * 4 * sizeof(float);
		else
			imageSize = texWidth * texHeight * 4;
		stagingBuffer = new StagingBuffer(_device, pixels, imageSize);
		stbi_image_free(pixels);

		// Create texture image and allocate memory (check if normal image or cube map)
		if(filename.find(".hdr") == std::string::npos || true)// If not .hdr file, normal image
		{
			_arrayLayers = 1;

			// Create 2D image
			_image = new Image(_device, texWidth, texHeight, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _mipLevels, VK_SAMPLE_COUNT_1_BIT);

			transitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

			// Copy from buffer to image
			copyBufferToImage(stagingBuffer->handle(), static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
		}
		else
		{
			_arrayLayers = 6;
			_height = 400;// TODO TESTING
			_width = 400;// TODO TESTING
			_mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(_height, _width)))) + 1;// TODO TESTING
			// Create cube map image
			_image = new Image(_device, _height, _width, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _mipLevels, VK_SAMPLE_COUNT_1_BIT, true);

			transitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

			// Copy from buffer to images
			copyBufferToCubeMapImages(stagingBuffer->handle(), static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
		}

		// Transition to read only while generating mipmaps
		generateMipmaps();

		delete stagingBuffer;
		stagingBuffer = nullptr;

		_imageView = new ImageView(_device, _image->handle(), format, VK_IMAGE_ASPECT_COLOR_BIT, _mipLevels, _arrayLayers==6);
		_sampler = new Sampler(_device, _mipLevels);

		Log::success("Texture", filename + " loaded successfully: " + std::to_string(_width) + " x " + std::to_string(_height));
	}

	Texture::Texture(std::shared_ptr<Device> device, std::shared_ptr<CommandPool> commandPool, VkExtent2D size):
		_device(device), _commandPool(commandPool), _arrayLayers(1)
	{
		_width = size.width;
		_height = size.height;
		_mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(_width, _height)))) + 1;

		VkDeviceSize imageSize = _width * _height * 4;
		std::vector<uint8_t> pixels(imageSize);

		StagingBuffer* stagingBuffer = new StagingBuffer(_device, pixels.data(), imageSize);

		_image = new Image(_device, _width, _height, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _mipLevels);

		transitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		copyBufferToImage(stagingBuffer->handle(), static_cast<uint32_t>(_width), static_cast<uint32_t>(_height));
		// Transition to read only while generating mipmaps
		generateMipmaps();

		delete stagingBuffer;
		stagingBuffer = nullptr;

		_imageView = new ImageView(_device, _image->handle(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, _mipLevels);
		_sampler = new Sampler(_device, _mipLevels);
	}

	Texture::Texture(std::shared_ptr<Device> device, std::shared_ptr<CommandPool> commandPool, unsigned char buffer[],  VkExtent2D size):
		_device(device), _commandPool(commandPool), _arrayLayers(1)
	{
		_width = size.width;
		_height = size.height;

		_mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(_width, _height)))) + 1;

		VkDeviceSize imageSize = _width * _height * 4;
		std::vector<uint8_t> pixels(imageSize);

		// Copy from buffer to image pixels buffer
		for(int y=0; y<_height; y++)
		{
			for(int x=0; x<_width; x++)
			{
				pixels[(x+y*_width)*4+0] = 255;
				pixels[(x+y*_width)*4+1] = 255;
				pixels[(x+y*_width)*4+2] = 255;
				pixels[(x+y*_width)*4+3] = buffer[x+y*_width];
			}
		}

		StagingBuffer* stagingBuffer = new StagingBuffer(_device, pixels.data(), imageSize);

		_image = new Image(_device, _width, _height, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _mipLevels);

		transitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		copyBufferToImage(stagingBuffer->handle(), static_cast<uint32_t>(_width), static_cast<uint32_t>(_height));
		// Transition to read only while generating mipmaps
		generateMipmaps();

		delete stagingBuffer;
		stagingBuffer = nullptr;

		_imageView = new ImageView(_device, _image->handle(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, _mipLevels);
		_sampler = new Sampler(_device, _mipLevels);
	}

	void Texture::updateTextureImage(std::vector<uint8_t> pixels)
	{
		std::vector<uint8_t> data(_width*_height*4);

		// Convert pixels to data (RGBA format)
		if((int)pixels.size() == _width*_height*4)
			data = pixels;
		else if((int)pixels.size() == _width*_height*3)
		{
			for(int x=0; x<_width; x++)
			{
				for(int y=0; y<_height; y++)
				{
					data[y*_width*4 + x*4+0] = pixels[y*_width*3 + x*3+0];
					data[y*_width*4 + x*4+1] = pixels[y*_width*3 + x*3+1];
					data[y*_width*4 + x*4+2] = pixels[y*_width*3 + x*3+2];
					data[y*_width*4 + x*4+3] = 255;
				}
			}
		}
		else
		{
			Log::warning("Texture", "Pixels size not supported by updateTextureImage.");
			return;
		}

		//for(int x=0; x<_width; x++)
		//{
		//	for(int y=0; y<_height; y++)
		//	{
		//		data[y*_width*4 + x*4+0] = 0;
		//		data[y*_width*4 + x*4+1] = 255;
		//		data[y*_width*4 + x*4+2] = 255;
		//		data[y*_width*4 + x*4+3] = 255;
		//	}
		//}

		VkDeviceSize imageSize = _width * _height * 4;
		StagingBuffer* stagingBuffer = new StagingBuffer(_device, data.data(), imageSize);

		transitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		copyBufferToImage(stagingBuffer->handle(), static_cast<uint32_t>(_width), static_cast<uint32_t>(_height));
		generateMipmaps();

		delete stagingBuffer;
		stagingBuffer = nullptr;
	}

	Texture::~Texture()
	{
		if(_image != nullptr)
		{
			delete _image;
			_image = nullptr;
		}

		if(_imageView != nullptr)
		{
			delete _imageView;
			_imageView = nullptr;
		}
		
		if(_sampler != nullptr)
		{
			delete _sampler;
			_sampler = nullptr;
		}
	}

	void Texture::transitionImageLayout(VkImageLayout newLayout)
	{
		// TODO also being used in depth buffer
		VkCommandBuffer commandBuffer = _commandPool->beginSingleTimeCommands();
		{
			VkImageMemoryBarrier barrier{};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = _image->getImageLayout();
			barrier.newLayout = newLayout;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

			barrier.image = _image->handle();
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = _mipLevels;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = _arrayLayers;

			VkPipelineStageFlags sourceStage;
			VkPipelineStageFlags destinationStage;

			if(_image->getImageLayout() == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
			{
				barrier.srcAccessMask = 0;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

				sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
				destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			} 
			else if(_image->getImageLayout() == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
			{
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			} 
			else if(_image->getImageLayout() == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
			{
				barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

				sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			} 
			else 
			{
				std::cout << BOLDRED << "[Texture]" << RESET << RED << " Unsupported layout transition!" << RESET << std::endl;
				exit(1);
			}

			vkCmdPipelineBarrier(
				commandBuffer,
				sourceStage, destinationStage,
				0,
				0, nullptr,
				0, nullptr,
				1, &barrier
			);
		}
		_commandPool->endSingleTimeCommands(commandBuffer);
		_image->setImageLayout(newLayout);
	}

	void Texture::copyBufferToImage(VkBuffer buffer, uint32_t width, uint32_t height)
	{
		VkCommandBuffer commandBuffer = _commandPool->beginSingleTimeCommands();
		{
			VkBufferImageCopy region{};
			region.bufferOffset = 0;
			region.bufferRowLength = 0;
			region.bufferImageHeight = 0;

			region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			region.imageSubresource.mipLevel = 0;
			region.imageSubresource.baseArrayLayer = 0;
			region.imageSubresource.layerCount = 1;

			region.imageOffset = {0, 0, 0};
			region.imageExtent = {
				width,
				height,
				1
			};

			vkCmdCopyBufferToImage(
				commandBuffer,
				buffer,
				_image->handle(),
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1,
				&region
			);
		}
		_commandPool->endSingleTimeCommands(commandBuffer);
	}

	void Texture::copyBufferToCubeMapImages(VkBuffer buffer, uint32_t width, uint32_t height)
	{
		VkCommandBuffer commandBuffer = _commandPool->beginSingleTimeCommands();
		{
			// Setup buffer copy regions for each face
			std::vector<VkBufferImageCopy> bufferCopyRegions;

			for(uint32_t face=0; face<6; face++)
			{
				for(uint32_t level=0; level<1; level++)
				{
					VkBufferImageCopy region{};
					region.bufferOffset = 0;
					region.bufferRowLength = width;
					region.bufferImageHeight = height;

					region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					region.imageSubresource.mipLevel = level;
					region.imageSubresource.baseArrayLayer = face;
					region.imageSubresource.layerCount = 1;

					region.imageOffset = {0, 0, 0};
					region.imageExtent = {
						_width >> level,
						_height >> level,
						1
					};
					bufferCopyRegions.push_back(region);
				}
			}

			vkCmdCopyBufferToImage(
				commandBuffer,
				buffer,
				_image->handle(),
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				bufferCopyRegions.size(),
				bufferCopyRegions.data()
			);
		}
		_commandPool->endSingleTimeCommands(commandBuffer);
	}

	void Texture::generateMipmaps()
	{
		// Check if image format supports linear blitting
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(_device->getPhysicalDevice()->handle(), _image->getFormat(), &formatProperties);
		if(!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
		{
			std::cout << BOLDRED << "[Texture]" << RESET << RED << " Texture image format does not support linear blitting!" << RESET << std::endl;
			exit(1);
		}

		VkCommandBuffer commandBuffer = _commandPool->beginSingleTimeCommands();
		{
			for(uint32_t layer=0; layer<_arrayLayers; layer++)
			{
				VkImageMemoryBarrier barrier{};
				barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				barrier.image = _image->handle();
				barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				barrier.subresourceRange.baseArrayLayer = layer;
				barrier.subresourceRange.layerCount = 1;
				barrier.subresourceRange.levelCount = 1;

				int32_t mipWidth = _width;
				int32_t mipHeight = _height;

				for(uint32_t i = 1; i < _mipLevels; i++)
				{
					barrier.subresourceRange.baseMipLevel = i - 1;
					barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
					barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

					vkCmdPipelineBarrier(commandBuffer,
						VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
						0, nullptr,
						0, nullptr,
						1, &barrier);

					VkImageBlit blit{};
					blit.srcOffsets[0] = {0, 0, 0};
					blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
					blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					blit.srcSubresource.mipLevel = i - 1;
					blit.srcSubresource.baseArrayLayer = layer;
					blit.srcSubresource.layerCount = 1;
					blit.dstOffsets[0] = {0, 0, 0};
					blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
					blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					blit.dstSubresource.mipLevel = i;
					blit.dstSubresource.baseArrayLayer = layer;
					blit.dstSubresource.layerCount = 1;

					vkCmdBlitImage(commandBuffer,
						_image->handle(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
						_image->handle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
						1, &blit,
						VK_FILTER_LINEAR);

					barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
					barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
					barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

					vkCmdPipelineBarrier(commandBuffer,
						VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
						0, nullptr,
						0, nullptr,
						1, &barrier);

					if (mipWidth > 1) mipWidth /= 2;
					if (mipHeight > 1) mipHeight /= 2;
				}

				barrier.subresourceRange.baseMipLevel = _mipLevels - 1;
				barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				vkCmdPipelineBarrier(commandBuffer,
					VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
					0, nullptr,
					0, nullptr,
					1, &barrier);
			}
		}
		_commandPool->endSingleTimeCommands(commandBuffer);
		_image->setImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}
}
