#ifndef IMAGE_H
#define IMAGE_H

#include "device.h"
#include "deviceMemory.h"
#include "buffer.h"

class Image
{
	public:
	Image(Device* device, VkExtent2D extent, VkFormat format);
	Image(Device* device, VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage);
	Image(Image&& other) noexcept;
	~Image();

	VkImage handle() const { return _image; }
	Device* device() const { return _device; }
	VkExtent2D extent() const { return _extent; }
	VkFormat format() const { return _format; }

	DeviceMemory allocateMemory(VkMemoryPropertyFlags properties) const;
	VkMemoryRequirements getMemoryRequirements() const;

	void transitionImageLayout(CommandPool* commandPool, VkImageLayout newLayout);
	void transitionCommand(CommandPool* commandPool, VkImageLayout newLayout, VkCommandBuffer commandBuffer);
	void copyFrom(CommandPool* commandPool, const Buffer* buffer);

	private:
		Device* _device;
		VkExtent2D _extent;
		VkFormat _format;
		VkImageLayout _imageLayout;
		VkImage _image;
};

#endif// IMAGE_H
