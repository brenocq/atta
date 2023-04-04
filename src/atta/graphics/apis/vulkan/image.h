//--------------------------------------------------
// Atta Graphics Module
// image.h
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_IMAGE_H
#define ATTA_GRAPHICS_APIS_VULKAN_IMAGE_H

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/device.h>
#include <atta/graphics/image.h>

namespace atta::graphics::vk {

class Image final : public graphics::Image {
  public:
    Image(const Image::CreateInfo& info, std::shared_ptr<Device> device);
    Image(const Image::CreateInfo& info, std::shared_ptr<Device> device, VkImage image);
    ~Image();

    void write(void* data) override;
    void resize(uint32_t width, uint32_t height, bool forceRecreate = false) override;

    void* getImGuiImage() override;
    VkImage getImageHandle() const;
    VkImageView getImageViewHandle() const;
    std::shared_ptr<Device> getDevice() const;

    static VkFormat convertFormat(Image::Format format);
    static Image::Format convertFormat(VkFormat format);
    static VkImageAspectFlags convertAspectFlags(Image::Format format);

  private:
    VkImage _image;
    VkImageView _imageView;
    std::shared_ptr<Device> _device;

    bool _destroyImage;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_IMAGE_H
