//--------------------------------------------------
// Atta Graphics Module
// image.cpp
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/image.h>

namespace atta::graphics::vk {

Image::Image(const graphics::Image::CreateInfo& info) : graphics::Image(info), _device(common::getDevice()), _destroyImage(true) {
    resize(_width, _height, true);
}

Image::Image(const graphics::Image::CreateInfo& info, std::shared_ptr<Device> device, VkImage image)
    : graphics::Image(info), _device(device), _destroyImage(false) {

    VkImageViewCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = image;
    createInfo.format = convertFormat(_format);
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

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
        LOG_ERROR("gfx::vk::Image", "Failed to create image view!");
} // namespace atta::graphics::vk

Image::~Image() {
    if (_image != VK_NULL_HANDLE && _destroyImage)
        vkDestroyImage(_device->getHandle(), _image, nullptr);
    if (_imageView != VK_NULL_HANDLE)
        vkDestroyImageView(_device->getHandle(), _imageView, nullptr);
}

void Image::write(void* data) {
    // TODO
}

void Image::resize(uint32_t width, uint32_t height, bool forceRecreate) {
    // TODO
}

void* Image::getImGuiImage() { return nullptr; }

VkImage Image::getImageHandle() const { return _image; }

VkImageView Image::getImageViewHandle() const { return _imageView; }

std::shared_ptr<Device> Image::getDevice() const { return _device; }

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
    }
    ASSERT(false, "Could not convert vulkan format [w]$0[] to atta format. Unknown image format", common::toString(format));
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

} // namespace atta::graphics::vk
