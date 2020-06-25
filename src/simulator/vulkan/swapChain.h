//--------------------------------------------------
// Robot Simulator
// swapChain.h
// Date: 24/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef SWAP_CHAIN_H
#define SWAP_CHAIN_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <cstdint> // Necessary for UINT32_MAX
#include "defines.h"
#include "device.h"
#include "imageView.h"
#include "window.h"
#include "helpers.h"

class SwapChain
{
	public:
	SwapChain(Device* device, Window* window);
	~SwapChain();

	VkSwapchainKHR handle() const { return _swapChain; }
    VkExtent2D getExtent() const { return _extent; }
    VkFormat getImageFormat() const { return _imageFormat; }
	std::vector<ImageView*> getImageViews() const { return _imageViews; }

	private:
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	void createImageViews();

	Device* _device;
	Window* _window;

	VkSwapchainKHR _swapChain;
	std::vector<VkImage> _images;
	std::vector<ImageView*> _imageViews;
	VkFormat _imageFormat;
    VkExtent2D _extent;
};

#endif// SWAP_CHAIN_H
