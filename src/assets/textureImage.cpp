#include "textureImage.h"
#include <cstring>

TextureImage::TextureImage(CommandPool* commandPool, const Texture& texture)
{
	// Create a host staging buffer and copy the image into it.
	const VkDeviceSize imageSize = texture.width() * texture.height() * 4;
	Device* device = commandPool->device();

	Buffer* stagingBuffer = new Buffer(device, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
	auto stagingBufferMemory = stagingBuffer->allocateMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	const auto data = stagingBufferMemory.mapMemory(0, imageSize);
	std::memcpy(data, texture.pixels(), imageSize);
	stagingBufferMemory.unmapMemory();

	// Create the device side image, memory, view and sampler.
	_image = new Image(device, VkExtent2D{ static_cast<uint32_t>(texture.width()), static_cast<uint32_t>(texture.height()) }, VK_FORMAT_R8G8B8A8_UNORM);
	_imageMemory = new DeviceMemory(_image->allocateMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
	_imageView = new ImageView(device, _image->handle(), _image->format(), VK_IMAGE_ASPECT_COLOR_BIT);
	_sampler = new Sampler(device, SamplerConfig());

	// Transfer the data to device side.
	_image->transitionImageLayout(commandPool, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	_image->copyFrom(commandPool, stagingBuffer);
	_image->transitionImageLayout(commandPool, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	// Delete the buffer before the memory
	delete stagingBuffer;
}

TextureImage::~TextureImage()
{
	delete _sampler;
	delete _imageView;
	delete _image;
	delete _imageMemory;
}
