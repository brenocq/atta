//--------------------------------------------------
// Atta Graphics Module
// swapChain.h
// Date: 2023-04-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_SWAP_CHAIN_H
#define ATTA_GRAPHICS_APIS_VULKAN_SWAP_CHAIN_H

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/device.h>
#include <atta/graphics/apis/vulkan/surface.h>

namespace atta::graphics::vk {

class SwapChain {
  public:
    SwapChain(std::shared_ptr<Device> device, std::shared_ptr<Surface> surface);
    ~SwapChain();

    // void copyImage(VkCommandBuffer commandBuffer, int imageIndex, Image* src, VkExtent2D extent, VkExtent2D offset);

    //---------- Getters and Setters ----------//
    VkSwapchainKHR getHandle() const;
    VkExtent2D getImageExtent() const;
    VkFormat getImageFormat() const;
    // std::vector<std::shared_ptr<ImageView>> getImageViews() const;
    std::vector<VkImage> getImages() const;

  private:
    struct SupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    SupportDetails querySwapChainSupport();
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    void createImageViews();

    VkSwapchainKHR _swapChain;
    std::shared_ptr<Device> _device;
    std::shared_ptr<Surface> _surface;
    std::vector<VkImage> _images;
    // std::vector<std::shared_ptr<ImageView>> _imageViews;
    VkFormat _imageFormat;
    VkExtent2D _extent;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_SWAP_CHAIN_H
