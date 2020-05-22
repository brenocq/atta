#include <algorithm>
#include <set>

#include "device.h"
#include "defines.h"

Device::Device(VkPhysicalDevice physicalDevice, Surface* surface):
	_physicalDevice(physicalDevice),
	_surface(surface)
{
	_requiredExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME,
							VK_NV_RAY_TRACING_EXTENSION_NAME};
	checkDeviceExtensions(); 

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, queueFamilies.data());
	
	// Find the queue families
	const auto graphicsFamily = findQueue(queueFamilies, "graphics", VK_QUEUE_GRAPHICS_BIT, 0);
	const auto computeFamily = findQueue(queueFamilies, "compute", VK_QUEUE_COMPUTE_BIT, VK_QUEUE_GRAPHICS_BIT);
	const auto transferFamily = findQueue(queueFamilies, "transfer", VK_QUEUE_TRANSFER_BIT, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);

	// Find the presentation queue (usually the same as graphics queue)
	const auto presentFamily = std::find_if(queueFamilies.begin(), queueFamilies.end(), [&](const VkQueueFamilyProperties& queueFamily)
	{
		VkBool32 presentSupport = false;
		const uint32_t i = static_cast<uint32_t>(&*queueFamilies.cbegin() - &queueFamily);
		vkGetPhysicalDeviceSurfaceSupportKHR(_physicalDevice, i, _surface->handle(), &presentSupport);
		return queueFamily.queueCount > 0 && presentSupport;
	});
	if (presentFamily == queueFamilies.end())
	{
		std::cout << BOLDRED << "[Device] Found no presentation queue" << RESET << std::endl;
		exit(1);
	}

	_graphicsFamilyIndex = static_cast<uint32_t>(graphicsFamily - queueFamilies.begin());
	_computeFamilyIndex = static_cast<uint32_t>(computeFamily - queueFamilies.begin());
	_presentFamilyIndex = static_cast<uint32_t>(presentFamily - queueFamilies.begin());
	_transferFamilyIndex = static_cast<uint32_t>(transferFamily - queueFamilies.begin());

	// Queues can be the same
	const std::set<uint32_t> uniqueQueueFamilies =
	{
		_graphicsFamilyIndex,
		_computeFamilyIndex,
		_presentFamilyIndex,
		_transferFamilyIndex
	};

	// Create queues
	float queuePriority = 1.0f;
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

	for (uint32_t queueFamilyIndex : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.fillModeNonSolid = true;
	deviceFeatures.samplerAnisotropy = true;

	VkPhysicalDeviceDescriptorIndexingFeaturesEXT indexingFeatures = {};
	indexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT;
	indexingFeatures.runtimeDescriptorArray = true;

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pNext = &indexingFeatures;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledLayerCount = static_cast<uint32_t>(_surface->instance()->validationLayers().size());
	createInfo.ppEnabledLayerNames = _surface->instance()->validationLayers().data();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(_requiredExtensions.size());
	createInfo.ppEnabledExtensionNames = _requiredExtensions.data();

	if(vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[Device] Failed to create logical device" << RESET << std::endl;
		exit(1);
	}

	vkGetDeviceQueue(_device, _graphicsFamilyIndex, 0, &_graphicsQueue);
	vkGetDeviceQueue(_device, _computeFamilyIndex, 0, &_computeQueue);
	vkGetDeviceQueue(_device, _presentFamilyIndex, 0, &_presentQueue);
	vkGetDeviceQueue(_device, _transferFamilyIndex, 0, &_transferQueue);
}

Device::~Device()
{
	if (_device != nullptr)
		vkDestroyDevice(_device, nullptr);
}

void Device::checkDeviceExtensions()
{
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(_physicalDevice, nullptr, &extensionCount, nullptr);// core dump here
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(_physicalDevice, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(_requiredExtensions.begin(), _requiredExtensions.end());

	for (const auto& extension : availableExtensions) 
	{
		requiredExtensions.erase(extension.extensionName);
	}

	if (!requiredExtensions.empty())
	{
		bool first = true;
		std::string extensions = "";

		for (const auto& extension : requiredExtensions)
		{
			if (!first)
			{
				extensions += ", ";
			}

			extensions += extension;
			first = false;
		}

		std::cout << BOLDRED << "[Device] Missing required extensions: " << RED << extensions << RESET << std::endl;
		exit(1);
	}
}

std::vector<VkQueueFamilyProperties>::const_iterator Device::findQueue(
	const std::vector<VkQueueFamilyProperties>& queueFamilies,
	const std::string& name,
	const VkQueueFlags requiredBits,
	const VkQueueFlags excludedBits)
{
	const auto family = std::find_if(queueFamilies.begin(), queueFamilies.end(), [requiredBits, excludedBits](const VkQueueFamilyProperties& queueFamily)
	{
		return 
			queueFamily.queueCount > 0 && 
			queueFamily.queueFlags & requiredBits &&
			!(queueFamily.queueFlags & excludedBits);
	});

	if (family == queueFamilies.end())
	{
		std::cout << BOLDRED << "[Device] Found no matching " << name << " queue" << RESET << std::endl;
		exit(1);
	}

	return family;
}
