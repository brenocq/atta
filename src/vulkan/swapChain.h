#ifndef SWAP_CHAIN_H
#define SWAP_CHAIN_H

#include <memory>
#include <vector>
#include "device.h"
#include "imageView.h"

struct SupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities{};
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class SwapChain
{
	public:
	SwapChain(Device* device);
	~SwapChain();

	VkSwapchainKHR handle() const { return _swapChain; }
	Device* device() const { return _device; }

	VkPhysicalDevice physicalDevice() const { return _physicalDevice; }
	uint32_t minImageCount() const { return _minImageCount; }
	const std::vector<VkImage>& images() const { return _images; }
	std::vector<ImageView*> imageViews() const { return _imageViews; }
	const VkExtent2D& extent() const { return _extent; }
	VkFormat format() const { return _format; }
	VkPresentModeKHR presentMode() const { return _presentMode; }

	private:
    SupportDetails querySwapChainSupport();
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	uint32_t chooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities);
		
	Device* _device;
	VkSwapchainKHR _swapChain;
	VkPhysicalDevice _physicalDevice;

	uint32_t _minImageCount;
	VkPresentModeKHR _presentMode;
	VkFormat _format;
	VkExtent2D _extent;
	std::vector<VkImage> _images;
	std::vector<ImageView*> _imageViews;
};

#endif// SWAP_CHAIN_H
