//--------------------------------------------------
// Robot Simulator
// swapChain.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VULKAN_SWAP_CHAIN_H
#define ATTA_GRAPHICS_VULKAN_SWAP_CHAIN_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstdint> // Necessary for UINT32_MAX
#include <atta/graphics/vulkan/device.h>
#include <atta/graphics/vulkan/image.h>
#include <atta/graphics/vulkan/imageView.h>
#include <atta/graphics/core/window.h>
#include <atta/graphics/vulkan/helpers.h>

namespace atta::vk
{
	class SwapChain
	{
		public:
			SwapChain(std::shared_ptr<Device> device, std::shared_ptr<Surface> surface);
			~SwapChain();

			void copyImage(VkCommandBuffer commandBuffer, int imageIndex, Image* src, VkExtent2D extent, VkExtent2D offset);

			//---------- Getters and Setters ----------//
			VkSwapchainKHR handle() const { return _swapChain; }
			VkExtent2D getImageExtent() const { return _extent; }
			VkFormat getImageFormat() const { return _imageFormat; }
			std::vector<std::shared_ptr<ImageView>> getImageViews() const { return _imageViews; }
			std::vector<VkImage> getImages() const { return _images; }

		private:
			SwapChainSupportDetails querySwapChainSupport();
			VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
			VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
			VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

			void createImageViews();

			VkSwapchainKHR _swapChain;
			std::shared_ptr<Device> _device;
			std::shared_ptr<Surface> _surface;
			std::vector<VkImage> _images;
			std::vector<std::shared_ptr<ImageView>> _imageViews;
			VkFormat _imageFormat;
			VkExtent2D _extent;
	};
}

#endif// ATTA_GRAPHICS_VULKAN_SWAP_CHAIN_H
