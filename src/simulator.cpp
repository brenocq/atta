#include "simulator.h"

Simulator::Simulator()
{
	initWindow();
	initInstance();
	initSurface();
	initDevice();
	initSwapChain();

	printPhysicalDevices();
}

Simulator::~Simulator()
{
}

void Simulator::initWindow()
{
	WindowConfig windowConfig = {};
	windowConfig.title = "Breno Queiroz Simulator";
	windowConfig.width = 1366;
	windowConfig.height = 768;
	windowConfig.cursorDisabled = false;
	windowConfig.fullscreen = false;
	windowConfig.resizable = false;

	_window = new Window(windowConfig);
}

void Simulator::initInstance()
{
	// Validation layers will only be used if ENABLE_VALIDATION_LAYERS is set to true
	const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

	_instance = new Instance(_window, validationLayers);
}

void Simulator::initSurface()
{
	_surface = new Surface(_instance);
}

void Simulator::initDevice()
{
	VkPhysicalDevice physicalDevice = choosePhysicalDevice();
	_device = new Device(physicalDevice, _surface);
	_commandPool = new CommandPool(_device, _device->graphicsFamilyIndex(), true);
}
void Simulator::initSwapChain()
{
	_swapChain = new SwapChain(_device);
}

void Simulator::run()
{
	_window->drawFrame = [this]() { drawFrame(); };
	_window->run();
}

void Simulator::drawFrame()
{

}

VkPhysicalDevice Simulator::choosePhysicalDevice()
{
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	const std::vector<VkPhysicalDevice>& physicalDevices = _instance->physicalDevices();

	auto result = std::find_if(physicalDevices.begin(), physicalDevices.end(), [](const VkPhysicalDevice& device)
	{
		// We want a device with geometry shader support
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		if (!deviceFeatures.geometryShader)
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

	if (result == physicalDevices.end())
	{
		std::cout << BOLDRED << "[Simulator] Cannot find a suitable device!" << RESET << std::endl;
		exit(1);
	}

	return *result;
}



std::string Simulator::getVersion(const uint32_t version)
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

std::string Simulator::getVersion(const uint32_t version, const uint32_t vendorId)
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

void Simulator::printPhysicalDevices()
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
