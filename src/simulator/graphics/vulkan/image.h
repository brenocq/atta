//--------------------------------------------------
// Robot Simulator
// image.h
// Date: 2020-07-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_IMAGE_H
#define ATTA_VK_IMAGE_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"
#include "commandPool.h"

namespace atta::vk
{
	class Image
	{
		public:
			Image(std::shared_ptr<Device> device, 
					uint32_t width, uint32_t height
					, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties
					, uint32_t mipLevels=1, VkSampleCountFlagBits numSamples=VK_SAMPLE_COUNT_1_BIT, bool isCubeMap=false);
			~Image();

			//---------- Getters and Setters ----------//
			std::shared_ptr<Device> getDevice() const { return _device; }
			VkImage handle() const { return _image; }
			VkImageLayout getImageLayout() const { return _layout; }
			VkExtent2D getExtent() const { return _extent; }
			VkFormat getFormat() const { return _format; }
			VkDeviceMemory getMemory() const { return _memory; }
			void setImageLayout(VkImageLayout layout) { _layout = layout; }
			std::vector<uint8_t> getBuffer(std::shared_ptr<CommandPool> commandPool);

		private:
			uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

			std::shared_ptr<Device> _device;
			VkImage _image;
			VkFormat _format;
			VkExtent2D _extent;
			VkImageLayout _layout;
			VkDeviceMemory _memory;
			uint32_t _mipLevels;
	};
}

#endif// ATTA_VK_IMAGE_H
