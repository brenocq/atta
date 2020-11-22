//--------------------------------------------------
// Robot Simulator
// image.h
// Date: 07/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"

class Image
{
	public:
		Image(Device* device, 
				uint32_t width, uint32_t height
				, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties
				, uint32_t mipLevels=1, VkSampleCountFlagBits numSamples=VK_SAMPLE_COUNT_1_BIT);
		~Image();

		//---------- Getters and Setters ----------//
		Device* getDevice() const { return _device; }
		VkImage handle() const { return _image; }
		VkImageLayout getImageLayout() const { return _layout; }
		VkFormat getFormat() const { return _format; }
		VkDeviceMemory getMemory() const { return _memory; }
		void setImageLayout(VkImageLayout layout) { _layout = layout; }

	private:
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		Device* _device;
		VkImage _image;
		VkFormat _format;
		VkDeviceMemory _memory;
		VkImageLayout _layout;
		uint32_t _mipLevels;
};

#endif// IMAGE_H
