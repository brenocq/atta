//--------------------------------------------------
// Robot Simulator
// device.cpp
// Date: 24/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "device.h"

Device::Device(PhysicalDevice* physicalDevice)
{
	_physicalDevice = physicalDevice;
	// Queues info
	QueueFamilyIndices indices = physicalDevice->findQueueFamilies();

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	// Device info
	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (ENABLE_VALIDATION_LAYERS) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	} else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice->handle(), &createInfo, nullptr, &_device) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[Device]" << RESET << RED << " Failed to create device!" << RESET << std::endl;
		exit(1);
	}
	vkGetDeviceQueue(_device, indices.graphicsFamily.value(), 0, &_graphicsQueue);	
	vkGetDeviceQueue(_device, indices.presentFamily.value(), 0, &_presentQueue);
}

Device::~Device()
{
	if(_device != nullptr)
	{
		vkDestroyDevice(_device, nullptr);
		_device = nullptr;
	}
}
