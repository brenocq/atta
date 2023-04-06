//--------------------------------------------------
// Atta Graphics Module
// swapChain.cpp
// Date: 2023-04-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/swapChain.h>

namespace atta::graphics::vk {

SwapChain::SwapChain(std::shared_ptr<Device> device, std::shared_ptr<Surface> surface) : _device(device), _surface(surface) {
    std::shared_ptr<PhysicalDevice> physicalDevice = _device->getPhysicalDevice();

    // Check swap chain support
    SupportDetails swapChainSupport = querySwapChainSupport();
    if (swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty())
        LOG_ERROR("gfx::vk::SwapChain", "Failed to create swapChain, physical device does not have swap chain support!");

    // Choose swap chain properties
    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    _imageFormat = surfaceFormat.format;
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    _extent = chooseSwapExtent(swapChainSupport.capabilities);

    // Choose image count
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        imageCount = swapChainSupport.capabilities.maxImageCount;

    //---------- Create swap chain ----------//
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = _surface->getHandle();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = _extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    // Select queue families
    PhysicalDevice::QueueFamilyIndices indices = physicalDevice->getQueueFamilyIndices();
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};
    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;     // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    // Setup
    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    // Check surface present support before creating
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(_device->getPhysicalDevice()->getHandle(), indices.presentFamily.value(), _surface->getHandle(),
                                         &presentSupport);
    if (!presentSupport) {
        LOG_ERROR("gfx::vk::SwapChain", "Current logical device does not support present to this surface!");
        return;
    }

    if (vkCreateSwapchainKHR(_device->getHandle(), &createInfo, nullptr, &_swapChain) != VK_SUCCESS) {
        LOG_ERROR("gfx::vk::SwapChain", "Failed to create swap chain!");
        return;
    }

    //---------- Create images ----------//
    std::vector<VkImage> swapChainImages;
    vkGetSwapchainImagesKHR(_device->getHandle(), _swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(_device->getHandle(), _swapChain, &imageCount, swapChainImages.data());
    for (VkImage img : swapChainImages) {
        gfx::Image::CreateInfo info{};
        info.width = _extent.width;
        info.height = _extent.height;
        info.format = vk::Image::convertFormat(_imageFormat);
        _images.push_back(std::make_shared<Image>(info, _device, img));
    }
}

SwapChain::~SwapChain() {
    if (_swapChain != VK_NULL_HANDLE)
        vkDestroySwapchainKHR(_device->getHandle(), _swapChain, nullptr);
}

// void SwapChain::copyImage(VkCommandBuffer commandBuffer, int imageIndex, Image* src, VkExtent2D extent, VkExtent2D offset) {
//     VkImageSubresourceRange subresourceRange;
//     subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//     subresourceRange.baseMipLevel = 0;
//     subresourceRange.levelCount = 1;
//     subresourceRange.baseArrayLayer = 0;
//     subresourceRange.layerCount = 1;
//
//     ImageMemoryBarrier::insert(commandBuffer, src->getHandle(), subresourceRange, VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
//                                VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
//
//     ImageMemoryBarrier::insert(commandBuffer, _images[imageIndex], subresourceRange, 0, VK_ACCESS_TRANSFER_WRITE_BIT,
//                                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
//
//     // Copy all image or only half (splits the screen in half)
//     VkImageCopy copyRegion;
//     copyRegion.srcSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
//     copyRegion.srcOffset = {0, 0, 0};
//     copyRegion.dstSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
//     copyRegion.dstOffset = {(int)offset.width, (int)offset.height, 0};
//     copyRegion.extent = {extent.width, extent.height, 1};
//
//     vkCmdCopyImage(commandBuffer, src->handle(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, _images[imageIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
//     1,
//                    &copyRegion);
//
//     ImageMemoryBarrier::insert(commandBuffer, src->handle(), subresourceRange, VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
//                                VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);
//     // Changed VK_IMAGE_LAYOUT_PRESENT_SRC_KHR to VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL (imgui)
//     ImageMemoryBarrier::insert(commandBuffer, _images[imageIndex], subresourceRange, VK_ACCESS_TRANSFER_WRITE_BIT, 0,
//                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
// }

VkSwapchainKHR SwapChain::getHandle() const { return _swapChain; }

VkExtent2D SwapChain::getImageExtent() const { return _extent; }

VkFormat SwapChain::getImageFormat() const { return _imageFormat; }

std::vector<std::shared_ptr<Image>> SwapChain::getImages() const { return _images; }

SwapChain::SupportDetails SwapChain::querySwapChainSupport() {
    SupportDetails details;
    VkSurfaceKHR surface = _surface->getHandle();
    VkPhysicalDevice device = _device->getPhysicalDevice()->getHandle();

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    // Surface formats
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    // Surface present modes
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats)
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return availableFormat;
    return availableFormats[0];
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes)
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            return availablePresentMode;
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        VkExtent2D actualExtent;
        actualExtent.width =
            std::clamp((uint32_t)_surface->getWindow()->getWidth(), capabilities.maxImageExtent.width, capabilities.minImageExtent.width);
        actualExtent.height =
            std::clamp((uint32_t)_surface->getWindow()->getHeight(), capabilities.maxImageExtent.height, capabilities.minImageExtent.height);
        return actualExtent;
    }
}

} // namespace atta::graphics::vk
