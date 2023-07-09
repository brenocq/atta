//--------------------------------------------------
// Atta Graphics Module
// image.h
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_IMAGE_H
#define ATTA_GRAPHICS_APIS_VULKAN_IMAGE_H

#include <atta/graphics/apis/vulkan/buffer.h>
#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/device.h>
#include <atta/graphics/image.h>

namespace atta::graphics::vk {

class Image final : public gfx::Image {
  public:
    /**
     * @brief Create image
     *
     * @param info Image create info
     */
    Image(const gfx::Image::CreateInfo& info);

    /**
     * @brief Create from vulkan image
     *
     * Used to create from swap chain image
     *
     * @param info Image create info
     * @param device Vulkan device
     * @param image Vulkan image
     */
    Image(const gfx::Image::CreateInfo& info, std::shared_ptr<Device> device, VkImage image);

    /**
     * @brief Destructor
     */
    ~Image();

    void write(uint8_t* data) override;
    void resize(uint32_t width, uint32_t height, bool forceRecreate = false) override;

    void* getImGuiImage() override;
    VkImage getImageHandle() const;
    VkImageView getImageViewHandle() const;
    VkSampler getSamplerHandle() const;
    std::shared_ptr<Device> getDevice() const;

    void copyFrom(std::shared_ptr<Buffer> buffer);

    static Image::Format supportedFormat(Image::Format format);
    static VkFormat convertFormat(Image::Format format);
    static Image::Format convertFormat(VkFormat format);
    static VkImageAspectFlags convertAspectFlags(Image::Format format);

  private:
    void createImage();
    void createImageView();
    void createSampler();
    void allocMemory();
    void destroy();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void transitionLayout(VkImageLayout newLayout);

    VkImage _image;
    VkImageView _imageView;
    VkSampler _sampler;
    VkDeviceMemory _memory;
    VkImageLayout _layout;
    std::shared_ptr<Device> _device;

    /**
     * @brief Supported vulkan format
     *
     * The GPU may not support _format, in that case _supportedFormat will differ from _format. The interface to the outside world will work as
     * _format, but under the hood _supportedFormat will be used and the necessary conversions will be done.
     *
     * For example, some GPUs may not support RGB, only RGBA, so the _supportedFormat will be RGBA, and the conversion from RGBA to RGB will be done
     * when reading the raw image
     */
    Format _supportedFormat;
    bool _destroyImage;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_IMAGE_H