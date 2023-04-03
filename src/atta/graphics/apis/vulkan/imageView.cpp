//--------------------------------------------------
// Atta Graphics Module
// imageView.cpp
// Date: 2023-04-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/imageView.h>

namespace atta::graphics::vk {

ImageView::ImageView(std::shared_ptr<Device> device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels,
                     bool isCubeMap)
    : _device(device), _mipLevels(mipLevels), _format(format) {
    VkImageViewCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = image;
    createInfo.format = format;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    if (isCubeMap) {
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
        createInfo.subresourceRange.layerCount = 6;
    } else {
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.subresourceRange.aspectMask = aspectFlags;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = _mipLevels;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
    }

    if (vkCreateImageView(_device->getHandle(), &createInfo, nullptr, &_imageView) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::ImageView", "Failed to create image view!");
}

ImageView::~ImageView() { vkDestroyImageView(_device->getHandle(), _imageView, nullptr); }

VkImageView ImageView::getHandle() const { return _imageView; }
std::shared_ptr<Device> ImageView::getDevice() const { return _device; }
VkFormat ImageView::getFormat() const { return _format; }

} // namespace atta::graphics::vk
