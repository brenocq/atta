//--------------------------------------------------
// Atta Graphics Module
// image.cpp
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/image.h>

#include <atta/graphics/apis/vulkan/commandPool.h>
#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/stagingBuffer.h>

// TODO should not have UI code here
#include <backends/imgui_impl_vulkan.h>

namespace atta::graphics::vk {

Image::Image(const gfx::Image::CreateInfo& info)
    : gfx::Image(info), _image(VK_NULL_HANDLE), _imageView(VK_NULL_HANDLE), _sampler(VK_NULL_HANDLE), _memory(VK_NULL_HANDLE),
      _imGuiDescriptorSet(VK_NULL_HANDLE), _device(common::getDevice()), _destroyImage(true) {
    _supportedFormat = supportedFormat(_format);
    resize(_width, _height, true);
}

Image::Image(const gfx::Image::CreateInfo& info, std::shared_ptr<Device> device, VkImage image)
    : gfx::Image(info), _image(image), _imageView(VK_NULL_HANDLE), _sampler(VK_NULL_HANDLE), _memory(VK_NULL_HANDLE),
      _imGuiDescriptorSet(VK_NULL_HANDLE), _device(device), _destroyImage(false) {
    _supportedFormat = supportedFormat(_format);
    createImageViews();
}

Image::~Image() { destroy(); }

void Image::write(uint8_t* data) {
    if (!_isCubemap) {
        uint8_t* finalData = data;
        uint32_t finalSize = _width * _height * Image::getPixelSize(_format);
        if (_format != _supportedFormat) {
            uint32_t co = Image::getNumChannels(_format);          // Num channels original
            uint32_t cs = Image::getNumChannels(_supportedFormat); // Num channels supported
            // Convert data from _format to _supportedFormat
            if (_supportedFormat == Image::Format::RGBA) {
                _supportedData.resize(_width * _height * cs);
                for (size_t y = 0; y < _height; y++)
                    for (size_t x = 0; x < _width; x++) {
                        size_t idx = x + y * _width;
                        size_t idxo = idx * co;
                        size_t idxs = idx * cs;
                        for (size_t c = 0; c < cs; c++)
                            _supportedData[idxs + c] = c < co ? data[idxo + c] : 255;
                    }
            } else if (_supportedFormat == Image::Format::RGBA32F) {
                _supportedData.resize(_width * _height * cs * sizeof(float));
                float* oDataF = (float*)data;
                float* sDataF = (float*)_supportedData.data();
                for (size_t y = 0; y < _height; y++)
                    for (size_t x = 0; x < _width; x++) {
                        size_t idx = x + y * _width;
                        size_t idxo = idx * co;
                        size_t idxs = idx * cs;
                        for (size_t c = 0; c < cs; c++)
                            sDataF[idxs + c] = c < co ? oDataF[idxo + c] : 1.0f;
                    }
            } else {
                LOG_ERROR("gfx::vk::Image", "Unknown format conversion when writing image. Image will not be written");
                return;
            }

            finalData = _supportedData.data();
            finalSize = _supportedData.size();
        }

        // Copy data to GPU
        std::shared_ptr<StagingBuffer> stagingBuffer = std::make_shared<StagingBuffer>(finalData, finalSize);
        VkCommandBuffer commandBuffer = common::getCommandPool()->beginSingleTimeCommands();
        {
            transitionLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
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
            vkCmdCopyBufferToImage(commandBuffer, stagingBuffer->getHandle(), _image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
            transitionLayout(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        }
        common::getCommandPool()->endSingleTimeCommands(commandBuffer);
    } else
        LOG_WARN("gfx::vk::Image", "Writing to cubemap image is not implemented yet. Image debug name: [w]$0[]", _debugName);
}

std::vector<uint8_t> Image::read(vec2i offset, vec2i size) {
    if (offset == vec2i() && size == vec2i())
        size = vec2i(_width, _height);
    std::vector<uint8_t> img(size.x * size.y * gfx::Image::getPixelSize(_supportedFormat));

    Buffer::CreateInfo info{};
    info.size = img.size();
    info.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    info.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(info);

    VkCommandBuffer commandBuffer = common::getCommandPool()->beginSingleTimeCommands();
    {
        transitionLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {int32_t(offset.x), int32_t(offset.y), 0};
        region.imageExtent = {uint32_t(size.x), uint32_t(size.y), 1};
        vkCmdCopyImageToBuffer(commandBuffer, _image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, buffer->getHandle(), 1, &region);
        transitionLayout(commandBuffer, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    }
    common::getCommandPool()->endSingleTimeCommands(commandBuffer);

    uint8_t* gpuData;
    vkMapMemory(_device->getHandle(), buffer->getMemoryHandle(), 0, img.size(), 0, (void**)&gpuData);
    for (size_t i = 0; i < img.size(); i++)
        img[i] = gpuData[i];

    return img;
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
    allocMemory();
    createImageViews();
    if (isColorFormat(_supportedFormat))
        createSampler();

    // Transfer data if specified
    if (_data)
        write(_data);
}

void* Image::getImGuiImage() {
    // Make sure image is readable
    VkCommandBuffer commandBuffer = common::getCommandPool()->beginSingleTimeCommands();
    transitionLayout(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    common::getCommandPool()->endSingleTimeCommands(commandBuffer);

    // Create ImGui descriptor set if necessary
    if (_imGuiDescriptorSet == VK_NULL_HANDLE)
        _imGuiDescriptorSet = ImGui_ImplVulkan_AddTexture(_sampler, _imageView, _layout);
    return static_cast<void*>(_imGuiDescriptorSet);
}

VkImage Image::getImageHandle() const { return _image; }

VkImageView Image::getImageViewHandle() const { return _imageView; }

VkImageView Image::getCubemapImageViewHandle(uint32_t layer) const {
    if (layer >= _cubemapImageViews.size()) {
        LOG_ERROR("gfx::vk::Image", "getCubemapImageViewHandle with layer index out of bounds. Image debug name: [w]$0[]", _debugName);
        return VK_NULL_HANDLE;
    }
    return _cubemapImageViews[layer];
}

VkSampler Image::getSamplerHandle() const { return _sampler; }

std::shared_ptr<Device> Image::getDevice() const { return _device; }

Image::Format Image::getSupportedFormat() const { return _supportedFormat; }

Image::Format Image::supportedFormat(Image::Format format) {
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties(common::getDevice()->getPhysicalDevice()->getHandle(), convertFormat(format), &formatProperties);

    if (format == Format::RGB && !(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT)) {
        Image::Format alternative = Format::RGBA;
        // LOG_WARN("gfx::vk::Image", "Format RGB is not supported, using RGBA instead");
        return alternative;
    } else if (format == Format::RGB32F && !(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT)) {
        Image::Format alternative = Format::RGBA32F;
        // LOG_WARN("gfx::vk::Image", "Format RGB32F is not supported, using RGBA32F instead");
        return alternative;
    }

    return format;
}

void Image::setLayout(VkImageLayout layout) { _layout = layout; }

Image::BaseType Image::getBaseType(Format format) {
    switch (format) {
        case Format::NONE:
            return BaseType::NONE;
        case Format::RED:
        case Format::RGB:
        case Format::BGR:
        case Format::RGBA:
        case Format::BGRA:
            return BaseType::FLOAT; // Vulkan UNORM format is used for these types
        case Format::RED32I:
            return BaseType::INT;
        case Format::RG16F:
        case Format::DEPTH32F:
        case Format::DEPTH24_STENCIL8:
        case Format::RGB16F:
        case Format::RGB32F:
        case Format::RGBA32F:
            return BaseType::FLOAT;
    }
    ASSERT(false, "Could not calculate base type. Unknown image format");
    return BaseType::NONE;
}

VkFormat Image::convertFormat(Image::Format format) {
    switch (format) {
        case Format::NONE:
            return VK_FORMAT_UNDEFINED;
        case Format::RED:
            return VK_FORMAT_R8_UNORM;
        case Format::RED32I:
            return VK_FORMAT_R32_SINT;
        case Format::RG16F:
            return VK_FORMAT_R16G16_SFLOAT;
        case Format::RGB:
            return VK_FORMAT_R8G8B8_UNORM;
        case Format::BGR:
            return VK_FORMAT_B8G8R8_UNORM;
        case Format::RGB16F:
            return VK_FORMAT_R16G16B16_SFLOAT;
        case Format::RGB32F:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case Format::RGBA:
            return VK_FORMAT_R8G8B8A8_UNORM;
        case Format::BGRA:
            return VK_FORMAT_B8G8R8A8_UNORM;
        case Format::RGBA32F:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        case Format::DEPTH32F:
            return VK_FORMAT_D32_SFLOAT;
        case Format::DEPTH24_STENCIL8:
            return VK_FORMAT_D24_UNORM_S8_UINT;
    }
    ASSERT(false, "Could not convert atta format to vulkan format. Unknown image format");
    return VK_FORMAT_UNDEFINED;
}

Image::Format Image::convertFormat(VkFormat format) {
    switch (format) {
        case VK_FORMAT_R8_UNORM:
            return Format::RED;
        case VK_FORMAT_R32_SINT:
            return Format::RED32I;
        case VK_FORMAT_R16G16_SFLOAT:
            return Format::RG16F;
        case VK_FORMAT_R8G8B8_UNORM:
            return Format::RGB;
        case VK_FORMAT_B8G8R8_UNORM:
            return Format::BGR;
        case VK_FORMAT_R16G16B16_SFLOAT:
            return Format::RGB16F;
        case VK_FORMAT_R32G32B32_SFLOAT:
            return Format::RGB32F;
        case VK_FORMAT_R8G8B8A8_UNORM:
            return Format::RGBA;
        case VK_FORMAT_B8G8R8A8_UNORM:
            return Format::BGRA;
        case VK_FORMAT_R32G32B32A32_SFLOAT:
            return Format::RGBA32F;
        case VK_FORMAT_D32_SFLOAT:
            return Format::DEPTH32F;
        case VK_FORMAT_D24_UNORM_S8_UINT:
            return Format::DEPTH24_STENCIL8;
        default:
            break;
    }
    ASSERT(false, "Could not convert vulkan format [w]$0[] to atta format. Unknown image format", common::toString(format));
    return Format::NONE;
}

VkImageAspectFlags Image::convertAspectFlags(Image::Format format) {
    switch (format) {
        case Format::NONE:
            return VK_IMAGE_ASPECT_NONE;
        case Format::RED:
        case Format::RG16F:
        case Format::RED32I:
        case Format::RGB:
        case Format::BGR:
        case Format::RGB16F:
        case Format::RGB32F:
        case Format::RGBA:
        case Format::BGRA:
        case Format::RGBA32F:
            return VK_IMAGE_ASPECT_COLOR_BIT;
        case Format::DEPTH32F:
            return VK_IMAGE_ASPECT_DEPTH_BIT;
        case Format::DEPTH24_STENCIL8:
            return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    }
    ASSERT(false, "Could not convert atta format to vulkan aspect flags. Unknown image format");
}

void Image::createImage() {
    VkImageCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    info.imageType = VK_IMAGE_TYPE_2D;
    info.extent = {_width, _height, 1};
    info.mipLevels = 1;
    // If this is a cubemap, use 6 layers, else 1.
    if (_isCubemap) {
        info.arrayLayers = 6;
        info.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
    } else {
        info.arrayLayers = 1;
        info.flags = 0;
    }
    info.format = convertFormat(_supportedFormat);
    info.tiling = VK_IMAGE_TILING_OPTIMAL;
    info.initialLayout = _layout = VK_IMAGE_LAYOUT_UNDEFINED;
    if (isColorFormat(_supportedFormat))
        info.usage =
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    else if (isDepthFormat(_supportedFormat))
        info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    info.samples = VK_SAMPLE_COUNT_1_BIT;
    if (vkCreateImage(_device->getHandle(), &info, nullptr, &_image) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::Image", "Failed to create image");
}

VkImageView Image::createImageView(uint32_t layer, uint32_t layerCount) {
    VkImageViewCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.image = _image;
    info.format = convertFormat(_supportedFormat);
    info.subresourceRange.aspectMask = convertAspectFlags(_supportedFormat);
    info.subresourceRange.baseMipLevel = 0;
    info.subresourceRange.levelCount = _mipLevels;
    info.subresourceRange.baseArrayLayer = layer;
    info.viewType = layerCount == 6 ? VK_IMAGE_VIEW_TYPE_CUBE : VK_IMAGE_VIEW_TYPE_2D;
    info.subresourceRange.layerCount = layerCount;

    VkImageView imageView;
    if (vkCreateImageView(_device->getHandle(), &info, nullptr, &imageView) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::Image", "Failed to create image view for image [w]$0[] (layer $0 layerCount $1)", _debugName, layer, layerCount);
    return imageView;
}

void Image::createImageViews() {
    if (_isCubemap) {
        _imageView = createImageView(0, 6);
        for (uint32_t i = 0; i < 6; i++)
            _cubemapImageViews[i] = createImageView(i, 1);
    } else {
        _imageView = createImageView();
    }
}

void Image::createSampler() {
    VkSamplerCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    // Mag/min filters
    info.magFilter = VK_FILTER_LINEAR;
    info.minFilter = VK_FILTER_LINEAR;
    // Address mode
    info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    // Anisotropy
    info.anisotropyEnable = VK_TRUE;
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(_device->getPhysicalDevice()->getHandle(), &properties);
    info.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    // Coordinates
    info.unnormalizedCoordinates = VK_FALSE;
    // Compare
    info.compareEnable = VK_FALSE;
    info.compareOp = VK_COMPARE_OP_ALWAYS;
    // Mipmapping
    info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    info.mipLodBias = 0.0f;
    info.minLod = 0.0f;
    info.maxLod = 0.0f;

    if (vkCreateSampler(_device->getHandle(), &info, nullptr, &_sampler) != VK_SUCCESS)
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
    vkDeviceWaitIdle(common::getDevice()->getHandle());
    if (_imGuiDescriptorSet != VK_NULL_HANDLE) {
        // XXX We would need to make sure this happens UI module is shut down, for now it is OK to not remove the texture because it is freed when the
        // UI descroptor pool is freed. This will fail if a lot of images are created and destroyed while the UI is running
        // ImGui_ImplVulkan_RemoveTexture(_imGuiDescriptorSet);
    }
    if (_imageView != VK_NULL_HANDLE)
        vkDestroyImageView(_device->getHandle(), _imageView, nullptr);
    for (const VkImageView& imageView : _cubemapImageViews)
        if (imageView != VK_NULL_HANDLE)
            vkDestroyImageView(_device->getHandle(), imageView, nullptr);
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

void populateLayoutStage(VkImageLayout layout, VkPipelineStageFlags* stage, VkAccessFlags* access) {
    switch (layout) {
        case VK_IMAGE_LAYOUT_UNDEFINED:
            *access = 0;
            *stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            break;
        case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
            *access = 0;
            *stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            break;
        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            *access = VK_ACCESS_SHADER_READ_BIT;
            *stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            break;
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            *access = VK_ACCESS_TRANSFER_WRITE_BIT;
            *stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            break;
        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            *access = VK_ACCESS_TRANSFER_READ_BIT;
            *stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            break;
        default:
            LOG_WARN("gfx::vk::Image", "Unsupported layout transition: [w]$0", layout);
            break;
    }
}

void Image::transitionLayout(VkCommandBuffer commandBuffer, VkImageLayout newLayout) {
    if (newLayout == _layout)
        return;

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
    barrier.subresourceRange.layerCount = _isCubemap ? 6 : 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    populateLayoutStage(_layout, &sourceStage, &barrier.srcAccessMask);
    populateLayoutStage(newLayout, &destinationStage, &barrier.dstAccessMask);

    vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    _layout = newLayout;
}

} // namespace atta::graphics::vk
