#include "swapChain.h"

SwapChain::SwapChain(Device* device):
	_device(device)
{
	// Swap chain support
	SupportDetails details = querySwapChainSupport();

	if (details.formats.empty() || details.presentModes.empty())
	{
		std::cout << BOLDRED << "[SwapChain] Empty swap chain support!" << RESET << std::endl;
		exit(1);
	}

	_physicalDevice = _device->physicalDevice();
	Surface* surface = _device->surface();

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(details.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(details.presentModes);
	VkExtent2D extent = chooseSwapExtent(details.capabilities);
	uint32_t imageCount = chooseImageCount(details.capabilities);

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface->handle();
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	createInfo.preTransform = details.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = nullptr;

	if (_device->graphicsFamilyIndex() != _device->presentFamilyIndex())
	{
		uint32_t queueFamilyIndices[] = { _device->graphicsFamilyIndex(), _device->presentFamilyIndex() };

		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	if(vkCreateSwapchainKHR(device->handle(), &createInfo, nullptr, &_swapChain) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[SwapChain] Failed to create swap chain!" << RESET << std::endl;
		exit(1);
	}

	_minImageCount = details.capabilities.minImageCount;
	_presentMode = presentMode;
	_format = surfaceFormat.format;
	_extent = extent;

	vkGetSwapchainImagesKHR(_device->handle(), _swapChain, &imageCount, nullptr);
	_images.resize(imageCount);
	vkGetSwapchainImagesKHR(_device->handle(), _swapChain, &imageCount, _images.data());
		
	_imageViews.reserve(_images.size());

	for (const auto image : _images)
	{
		_imageViews.push_back(std::make_unique<ImageView>(device, image, _format, VK_IMAGE_ASPECT_COLOR_BIT));
	}

}

SwapChain::~SwapChain()
{
	_imageViews.clear();

	if (_swapChain != nullptr)
		vkDestroySwapchainKHR(_device->handle(), _swapChain, nullptr);
}

SupportDetails SwapChain::querySwapChainSupport()
{
	SupportDetails details;
	VkDevice device = _device->handle();
	VkSurfaceKHR surface = _device->surface()->handle();

	// Capabilities
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_physicalDevice, surface, &details.capabilities);

	// Formats
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, surface, &formatCount, nullptr);
	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, surface, &formatCount, details.formats.data());
	}

	// Present modes
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice, surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}
VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats) 
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB 
				&& availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
		{
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for(const auto& availablePresentMode : availablePresentModes) 
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
		{
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != UINT32_MAX) 
	{
		return capabilities.currentExtent;
	} 
	else 
	{
		VkExtent2D actualExtent = _device->surface()->instance()->window()->windowSize();

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

uint32_t SwapChain::chooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities)
{
	uint32_t imageCount = capabilities.minImageCount+1; 
	
	if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
	{
		imageCount = capabilities.maxImageCount;
	}

	return imageCount;
}
