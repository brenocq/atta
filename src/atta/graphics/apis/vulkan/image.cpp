//--------------------------------------------------
// Atta Graphics Module
// image.cpp
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/image.h>

#include <atta/graphics/apis/vulkan/commandPool.h>
#include <atta/graphics/apis/vulkan/stagingBuffer.h>

namespace atta::graphics::vk {

Image::Image(const graphics::Image::CreateInfo& info)
    : graphics::Image(info), _image(VK_NULL_HANDLE), _imageView(VK_NULL_HANDLE), _sampler(VK_NULL_HANDLE), _memory(VK_NULL_HANDLE),
      _device(common::getDevice()), _destroyImage(true) {
    resize(_width, _height, true);
}

Image::Image(const graphics::Image::CreateInfo& info, std::shared_ptr<Device> device, VkImage image)
    : graphics::Image(info), _image(image), _imageView(VK_NULL_HANDLE), _sampler(VK_NULL_HANDLE), _memory(VK_NULL_HANDLE), _device(device),
      _destroyImage(false) {
    createImageView();
}

Image::~Image() { destroy(); }

void Image::write(void* data) {
    size_t size = _width * _height * Image::getPixelSize(_format);
    std::shared_ptr<StagingBuffer> stagingBuffer = std::make_shared<StagingBuffer>(_data, size);
    transitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copyFrom(stagingBuffer);
    transitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void Image::resize(uint32_t width, uint32_t height, bool forceRecreate) {
    // Check if size was not changed
    if (!forceRecreate && (_width == width && _height == height))
        return;
    _width = width;
    _height = height;

    // Destroy old handles
    destroy();

    // Create new handles
    createImage();
    createImageView();
    if (isColorFormat(_format))
        createSampler();
    allocMemory();

    // Transfer data if specified
    if (_data)
        write(_data);
}

void* Image::getImGuiImage() { return nullptr; }

VkImage Image::getImageHandle() const { return _image; }

VkImageView Image::getImageViewHandle() const { return _imageView; }

VkSampler Image::getSamplerHandle() const { return _sampler; }

std::shared_ptr<Device> Image::getDevice() const { return _device; }

void Image::copyFrom(std::shared_ptr<Buffer> buffer) {
    VkCommandBuffer commandBuffer = common::getCommandPool()->beginSingleTimeCommands();
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
        region.imageExtent = {_width, _height, 1};

        vkCmdCopyBufferToImage(commandBuffer, buffer->getHandle(), _image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
    }
    common::getCommandPool()->endSingleTimeCommands(commandBuffer);
}

VkFormat Image::convertFormat(Image::Format format) {
    switch (format) {
        case Format::NONE:
            break;
        case Format::RED:
            return VK_FORMAT_R8_UINT;
        case Format::RED32I:
            return VK_FORMAT_R32_SINT;
        case Format::RG16F:
            return VK_FORMAT_R16G16_SFLOAT;
        case Format::RGB:
            return VK_FORMAT_B8G8R8_UNORM;
        case Format::RGB16F:
            return VK_FORMAT_R16G16B16_SFLOAT;
        case Format::RGBA:
            return VK_FORMAT_B8G8R8A8_UNORM;
        case Format::DEPTH32F:
            return VK_FORMAT_D32_SFLOAT;
        case Format::DEPTH24_STENCIL8:
            return VK_FORMAT_D24_UNORM_S8_UINT;
    }
    ASSERT(false, "Could not convert atta format to vulkan format. Unknown image format");
}

Image::Format Image::convertFormat(VkFormat format) {
    switch (format) {
        case VK_FORMAT_R8_UINT:
            return Format::RED;
        case VK_FORMAT_R32_SINT:
            return Format::RED32I;
        case VK_FORMAT_R16G16_SFLOAT:
            return Format::RG16F;
        case VK_FORMAT_B8G8R8_UNORM:
            return Format::RGB;
        case VK_FORMAT_R16G16B16_SFLOAT:
            return Format::RGB16F;
        case VK_FORMAT_B8G8R8A8_UNORM:
            return Format::RGBA;
        case VK_FORMAT_D32_SFLOAT:
            return Format::DEPTH32F;
        case VK_FORMAT_D24_UNORM_S8_UINT:
            return Format::DEPTH24_STENCIL8;
        default:
            ASSERT(false, "Could not convert vulkan format [w]$0[] to atta format. Unknown image format", common::toString(format));
            return Format::NONE;
    }
}

VkImageAspectFlags Image::convertAspectFlags(Image::Format format) {
    switch (format) {
        case Format::NONE:
            return VK_IMAGE_ASPECT_NONE;
        case Format::RED:
        case Format::RG16F:
        case Format::RED32I:
        case Format::RGB:
        case Format::RGB16F:
        case Format::RGBA:
            return VK_IMAGE_ASPECT_COLOR_BIT;
        case Format::DEPTH32F:
            return VK_IMAGE_ASPECT_DEPTH_BIT;
        case Format::DEPTH24_STENCIL8:
            return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    }
    ASSERT(false, "Could not convert atta format to vulkan aspect flags. Unknown image format");
}

void Image::createImage() {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = _width;
    imageInfo.extent.height = _height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = convertFormat(_format);
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = _layout = VK_IMAGE_LAYOUT_UNDEFINED;
    if (isColorFormat(_format))
        imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    else if (isDepthFormat(_format))
        imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0;
    if (vkCreateImage(_device->getHandle(), &imageInfo, nullptr, &_image) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::Image", "Failed to create image");
}

void Image::createImageView() {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = _image;
    createInfo.format = convertFormat(_format);

    if (_isCubemap) {
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
        createInfo.subresourceRange.layerCount = 6;
    } else {
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.subresourceRange.aspectMask = convertAspectFlags(_format);
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = _mipLevels;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
    }

    if (vkCreateImageView(_device->getHandle(), &createInfo, nullptr, &_imageView) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::Image", "Failed to create image view");
}

void Image::createSampler() {
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    // Mag/min filters
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    // Address mode
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    // Anisotropy
    samplerInfo.anisotropyEnable = VK_TRUE;
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(_device->getPhysicalDevice()->getHandle(), &properties);
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    // Coordinates
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    // Compare
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    // Mipmapping
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if (vkCreateSampler(_device->getHandle(), &samplerInfo, nullptr, &_sampler) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::Image", "Failed to create texture sampler");
}

void Image::allocMemory() {
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(_device->getHandle(), _image, &memRequirements);
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(_device->getHandle(), &allocInfo, nullptr, &_memory) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::Image", "Failed to allocate image memory");
    vkBindImageMemory(_device->getHandle(), _image, _memory, 0);
}

void Image::destroy() {
    if (_imageView != VK_NULL_HANDLE)
        vkDestroyImageView(_device->getHandle(), _imageView, nullptr);
    if (_sampler != VK_NULL_HANDLE)
        vkDestroySampler(_device->getHandle(), _sampler, nullptr);
    if (_image != VK_NULL_HANDLE && _destroyImage)
        vkDestroyImage(_device->getHandle(), _image, nullptr);
    if (_memory != VK_NULL_HANDLE && _destroyImage)
        vkFreeMemory(_device->getHandle(), _memory, nullptr);
}

uint32_t Image::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(_device->getPhysicalDevice()->getHandle(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;

    LOG_ERROR("gfx::vk::Image", "Failed to find suitable memory type");
    return 0;
}

void Image::transitionLayout(VkImageLayout newLayout) {

    VkCommandBuffer commandBuffer = common::getCommandPool()->beginSingleTimeCommands();
    {
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = _layout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = _image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;
        if (_layout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } else
            LOG_WARN("gfx::vk::Image", "Unsupported layout transition");

        vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    }
    common::getCommandPool()->endSingleTimeCommands(commandBuffer);
    _layout = newLayout;
}

} // namespace atta::graphics::vk
