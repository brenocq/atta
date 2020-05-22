#include "physicalDevice.h"

PhysicalDevice::PhysicalDevice(Instance* instance):
	_instance(instance)
{
	printPhysicalDevices();
	choosePhysicalDevice();
}

PhysicalDevice::~PhysicalDevice()
{

}

VkPhysicalDevice PhysicalDevice::choosePhysicalDevice()
{
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	const std::vector<VkPhysicalDevice>& physicalDevices = _instance->physicalDevices();

	// Check if any of the physical devices have geometry shader and graphics queue support 
	auto result = std::find_if(physicalDevices.begin(), physicalDevices.end(), [](const VkPhysicalDevice& device)
	{
		// We want a device with geometry shader support
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		if(!deviceFeatures.geometryShader)
		{
			return false;
		}

		// We want a device with a graphics queue
		// Get device queues
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		// Check if there is some graphics queue
		auto graphicsQueue = std::find_if(queueFamilies.begin(), queueFamilies.end(), [](VkQueueFamilyProperties queueFamily)
		{
			return queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT;
		});

		return graphicsQueue != queueFamilies.end();
	});

	if(result == physicalDevices.end())
	{
		std::cout << BOLDRED << "[Simulator] Cannot find a suitable device!" << RESET << std::endl;
		exit(1);
	}

	_physicalDevice = *result;
}

void PhysicalDevice::printPhysicalDevices()
{

	std::cout << BOLDWHITE << "GPUs with Vulkan support: " << RESET << std::endl;

	const std::vector<VkPhysicalDevice> physicalDevices = _instance->physicalDevices();

	for (const auto& device : physicalDevices)
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
