//--------------------------------------------------
// Robot Simulator
// swapChain.cpp
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "swapChain.h"
#include "imageMemoryBarrier.h"
#include "simulator/helpers/log.h"

namespace atta::vk
{
	SwapChain::SwapChain(std::shared_ptr<Device> device, std::shared_ptr<Surface> surface):
		_device(device), _surface(surface)
	{
		std::shared_ptr<PhysicalDevice> physicalDevice = _device->getPhysicalDevice();

		SwapChainSupportDetails swapChainSupport = querySwapChainSupport();
		if(swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty())
		{
			Log::error("SwapChain", "Failed to create swapChain, physical device does not have swap chain support!");
		}

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = _surface->handle();

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;;

		QueueFamilyIndices indices = physicalDevice->getQueueFamilyIndices();
		uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

		if(indices.graphicsFamily != indices.presentFamily)
		{
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

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		// Check surface present support before creating
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(_device->getPhysicalDevice()->handle(), indices.presentFamily.value(), _surface->handle(), &presentSupport);
		if(!presentSupport) 
		{
			Log::error("SwapChain", "Current logical device does not support present to this surface!");
		}

		if(vkCreateSwapchainKHR(_device->handle(), &createInfo, nullptr, &_swapChain) != VK_SUCCESS)
		{
			Log::error("SwapChain", "Failed to create swap chain!");
			exit(1);
		}

		vkGetSwapchainImagesKHR(_device->handle(), _swapChain, &imageCount, nullptr);
		_images.resize(imageCount);
		vkGetSwapchainImagesKHR(_device->handle(), _swapChain, &imageCount, _images.data());

		_imageFormat = surfaceFormat.format;
		_extent = extent;

		createImageViews();
	}

	SwapChain::~SwapChain()
	{
		if(_swapChain != nullptr)
		{
			vkDestroySwapchainKHR(_device->handle(), _swapChain, nullptr);
			_swapChain = nullptr;
		}
	}

	void SwapChain::copyImage(VkCommandBuffer commandBuffer, int imageIndex, Image* src, VkExtent2D extent, VkExtent2D offset)
	{
		VkImageSubresourceRange subresourceRange;
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.baseArrayLayer = 0;
		subresourceRange.layerCount = 1;

		ImageMemoryBarrier::insert(commandBuffer, src->handle(), subresourceRange, 
			VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

		ImageMemoryBarrier::insert(commandBuffer, _images[imageIndex], subresourceRange, 
			0, VK_ACCESS_TRANSFER_WRITE_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		// Copy all image or only half (splits the screen in half)
		VkImageCopy copyRegion;
		copyRegion.srcSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
		copyRegion.srcOffset = { 0, 0, 0 };
		copyRegion.dstSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
		copyRegion.dstOffset = { (int)offset.width, (int)offset.height, 0 };
		copyRegion.extent = { extent.width, extent.height, 1 };

		vkCmdCopyImage(commandBuffer,
			src->handle(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			_images[imageIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &copyRegion);

		ImageMemoryBarrier::insert(commandBuffer, src->handle(), subresourceRange, 
			VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);
		// Changed VK_IMAGE_LAYOUT_PRESENT_SRC_KHR to VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL (imgui)
		ImageMemoryBarrier::insert(commandBuffer, _images[imageIndex], subresourceRange, VK_ACCESS_TRANSFER_WRITE_BIT,
			0, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
			//0, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
		// Change layouts back
	}

	SwapChainSupportDetails SwapChain::querySwapChainSupport()
	{
		SwapChainSupportDetails details;
		VkSurfaceKHR surface = _surface->handle();
		VkPhysicalDevice device = _device->getPhysicalDevice()->handle();

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		// Surface formats
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if(formatCount != 0) 
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		// Surface present modes
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if(presentModeCount != 0) 
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for(const auto& availableFormat : availableFormats) 
		{
			// Could not use VK_FORMAT_B8G8R8A8_SRGB with raytracing pipeline, using VK_FORMAT_B8G8R8A8_UNORM instead
			//if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
			if(availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
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
			if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
			{
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;	
	}

	VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if(capabilities.currentExtent.width != UINT32_MAX) 
		{
			return capabilities.currentExtent;
		} 
		else 
		{
			VkExtent2D actualExtent = _surface->getWindow()->getExtent();

			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}

	void SwapChain::createImageViews()
	{
		_imageViews.resize(_images.size());

		for(size_t i = 0; i < _imageViews.size(); i++) 
		{
			_imageViews[i] = std::make_shared<ImageView>(_device, _images[i], _imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
		}
	}
}
