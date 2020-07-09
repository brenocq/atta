//--------------------------------------------------
// Robot Simulator
// physicalDevice.cpp
// Date: 24/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "physicalDevice.h"

PhysicalDevice::PhysicalDevice(Instance* instance, Surface* surface)
{
	checkArguments(instance, surface);
	_instance = instance;
	_surface = surface;
	_physicalDevice = VK_NULL_HANDLE;

	// Get device count
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(_instance->handle(), &deviceCount, nullptr);
	
	// Check if some device was found
	if(deviceCount == 0) 
	{
		std::cout << BOLDRED << "[Physical Device]" << RESET << RED << " Failed to find GPUs with Vulkan support!" << RESET << std::endl;
		exit(1);
	}

	// Get physical devices
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(_instance->handle(), &deviceCount, devices.data());
	printPhysicalDevices(devices);

	for(const auto& device : devices) 
	{
		if(isDeviceSuitable(device)) 
		{
			_physicalDevice = device;
			break;
		}
	}

	if(_physicalDevice == VK_NULL_HANDLE) 
	{
		std::cout << BOLDRED << "[Physical Device]" << RESET << RED << " Failed to find a suitable GPU!" << RESET << std::endl;
		exit(1);
	}
}

PhysicalDevice::~PhysicalDevice()
{
}

bool PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device) 
{
	QueueFamilyIndices indices = findQueueFamilies(device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) 
	{
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) 
{
	// Get device extensions
	uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

QueueFamilyIndices PhysicalDevice::findQueueFamilies(VkPhysicalDevice device) 
{
    // Logic to find graphics queue family
	QueueFamilyIndices indices;	

	// Get queue families
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for(const auto& queueFamily : queueFamilies) 
	{
		if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _surface->handle(), &presentSupport);
		if (presentSupport) 
		{
			indices.presentFamily = i;
		}


		if(indices.isComplete()) 
		{
			break;
		}

		i++;
	}

	return indices;
}

SwapChainSupportDetails PhysicalDevice::querySwapChainSupport()
{
	return querySwapChainSupport(_physicalDevice);
}

SwapChainSupportDetails PhysicalDevice::querySwapChainSupport(VkPhysicalDevice device)
{
	VkSurfaceKHR surface = _surface->handle();

	SwapChainSupportDetails details;
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

QueueFamilyIndices PhysicalDevice::findQueueFamilies()
{
	return findQueueFamilies(_physicalDevice);
}

void PhysicalDevice::checkArguments(Instance* instance, Surface* surface)
{
	if(instance != nullptr)
	{
		if(instance->handle() == VK_NULL_HANDLE)
		{
			std::cout << BOLDRED << "[Physical Device]" << RESET << RED << " Instance handle can't be null!" << RESET << std::endl;
			exit(1);
		}
	}
	else
	{
		std::cout << BOLDRED << "[Physical Device]" << RESET << RED << " Instance can't be null!" << RESET << std::endl;
		exit(1);
	}

	if(surface != nullptr)
	{
		if(surface->handle() == VK_NULL_HANDLE)
		{
			std::cout << BOLDRED << "[Physical Device]" << RESET << RED << " Surface handle can't be null!" << RESET << std::endl;
			exit(1);
		}
	}
	else
	{
		std::cout << BOLDRED << "[Physical Device]" << RESET << RED << " Surface can't be null!" << RESET << std::endl;
		exit(1);
	}
}

void PhysicalDevice::printPhysicalDevices(std::vector<VkPhysicalDevice> physicalDevices)
{

	std::cout << BOLDWHITE << "GPUs with Vulkan support: " << RESET << std::endl;

	for(VkPhysicalDevice device : physicalDevices)
	{
		VkPhysicalDeviceProperties prop;
		vkGetPhysicalDeviceProperties(device, &prop);

		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(device, &features);

		std::string vendorId = "Unknown";
		switch (prop.vendorID)
		{
			case 0x1002:
				vendorId = "AMD";
				break;
			case 0x1010:
				vendorId = "ImgTec";
				break;
			case 0x10DE:
				vendorId = "NVIDIA";
				break;
			case 0x13B5:
				vendorId = "ARM";
				break;
			case 0x5143:
				vendorId = "Qualcomm";
				break;
			case 0x8086:
				vendorId = "Intel";
				break;
			default:
				break;
		}

		std::string deviceType = "Unknown";
		switch (prop.deviceType)
		{
			case VK_PHYSICAL_DEVICE_TYPE_OTHER:
				deviceType = "Other";
				break;
			case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
				deviceType = "Integrated GPU";
				break;
			case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
				deviceType = "Discrete GPU";
				break;
			case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
				deviceType = "Virtual GPU";
				break;
			case VK_PHYSICAL_DEVICE_TYPE_CPU:
				deviceType = "CPU";
			default:
				break;
		}

		std::cout << "- [" << prop.deviceID << "] ";
		std::cout << BOLDCYAN << vendorId << " " << prop.deviceName << RESET;
		std::cout << " (";
		std::cout << deviceType << ": ";
		std::cout << "vulkan " << getVersion(prop.apiVersion) << ", ";
		std::cout << "driver " << getVersion(prop.driverVersion, prop.vendorID);
		std::cout << ")" << std::endl;
	}
	std::cout << std::endl;
}

std::string PhysicalDevice::getVersion(const uint32_t version)
{
	// Convert version to human readable
	std::stringstream ss;
	ss << VK_VERSION_MAJOR(version);
	ss << ".";
	ss << VK_VERSION_MINOR(version);
	ss << ".";
	ss << VK_VERSION_PATCH(version);

	return ss.str();
}

std::string PhysicalDevice::getVersion(const uint32_t version, const uint32_t vendorId)
{
	// Convert version to human readable
	std::stringstream ss;
	ss << VK_VERSION_MAJOR(version);
	ss << ".";
	ss << (VK_VERSION_MINOR(version) >> (vendorId == 0x10DE ? 2 : 0));
	ss << ".";
	ss << (VK_VERSION_PATCH(version) >> (vendorId == 0x10DE ? 4 : 0));
	return ss.str();
}
