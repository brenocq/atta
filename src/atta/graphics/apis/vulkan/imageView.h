//--------------------------------------------------
// Atta Graphics Module
// imageView.h
// Date: 2023-04-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_IMAGE_VIEW_H
#define ATTA_GRAPHICS_APIS_VULKAN_IMAGE_VIEW_H
#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/device.h>

namespace atta::graphics::vk {

class ImageView {
  public:
    ImageView(std::shared_ptr<Device> device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels = 1,
              bool isCubeMap = false);
    ~ImageView();

    VkImageView getHandle() const;
    std::shared_ptr<Device> getDevice() const;
    VkFormat getFormat() const;

  private:
    VkImageView _imageView;
    std::shared_ptr<Device> _device;
    uint32_t _mipLevels;
    VkFormat _format;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_IMAGE_VIEW_H
