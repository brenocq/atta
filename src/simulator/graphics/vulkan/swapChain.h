//--------------------------------------------------
// Robot Simulator
// swapChain.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_SWAP_CHAIN_H
#define ATTA_VK_SWAP_CHAIN_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstdint> // Necessary for UINT32_MAX
#include "defines.h"
#include "device.h"
#include "image.h"
#include "imageView.h"
#include "simulator/graphics/core/window.h"
#include "helpers.h"

namespace atta::vk
{
	class SwapChain
	{
		public:
			SwapChain(std::shared_ptr<Device> device, std::shared_ptr<Window> window);
			~SwapChain();

			void copyImage(VkCommandBuffer commandBuffer, int imageIndex, Image* src, VkExtent2D extent, VkExtent2D offset);

			//---------- Getters and Setters ----------//
			VkSwapchainKHR handle() const { return _swapChain; }
			VkExtent2D getExtent() const { return _extent; }
			VkFormat getImageFormat() const { return _imageFormat; }
			std::vector<ImageView*> getImageViews() const { return _imageViews; }
			std::vector<VkImage> getImages() const { return _images; }

		private:
			VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
			VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
			VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

			void createImageViews();

			std::shared_ptr<Device> _device;
			std::weak_ptr<Window> _window;

			VkSwapchainKHR _swapChain;
			std::vector<VkImage> _images;
			std::vector<ImageView*> _imageViews;
			VkFormat _imageFormat;
			VkExtent2D _extent;
	};
}

#endif// ATTA_VK_SWAP_CHAIN_H
