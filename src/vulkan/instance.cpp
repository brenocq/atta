#include "instance.h"
#include "defines.h"

Instance::Instance(Window* window, const std::vector<const char*> validationLayers):
	_window(window),
	_validationLayers(validationLayers)
{
	// Check vulkan validation layer support
	if (ENABLE_VALIDATION_LAYERS && !checkVulkanValidationLayerSupport()) {
		std::cout << BOLDRED << "[Instance] Validation layers requested, but not available!";
		exit(1);
	}

	// Application info
	// Data for developer convenience
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Breno Queiroz Simulator";   // Custom application name
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);  // Custom application version
	appInfo.pEngineName = "No Engine";                      // Custom engine name
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);       // Custom engine version
	appInfo.apiVersion = VK_API_VERSION_1_1;                // Vulkan version

	// Vulkan instance info
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	// Populate vector to hold instance required extensions
	std::vector<const char*> requiredExtensions = getInstanceRequiredExtensions(); 
	// Check if all required extensions are supported
	getVulkanExtensions();
	if(!checkInstanceExtensionSupport(requiredExtensions))
	{
		std::cout << BOLDRED << "[Instance] Failed to find required extension!" << RESET << std::endl;
		exit(1);
	}

	createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
	createInfo.ppEnabledExtensionNames = requiredExtensions.data();

	if(ENABLE_VALIDATION_LAYERS) 
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
		createInfo.ppEnabledLayerNames = _validationLayers.data();
	} 
	else 
	{
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	// Create the Vulkan instance
	if(vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS) 
	{
		std::cout << BOLDRED << "[Instance] Failed to create instance!" << RESET << std::endl;
		exit(1);
	}

	getVulkanDevices();
}

Instance::~Instance()
{
	if(_instance != nullptr)
		vkDestroyInstance(_instance, nullptr);
}

bool Instance::checkVulkanValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : _validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

std::vector<const char*> Instance::getInstanceRequiredExtensions()
{
	// Populate vector to hold instance required extensions
	std::vector<const char*> requiredExtensions = _window->getRequiredExtensions();
	// If validation enabled, add extension to report validation debug info
	if(ENABLE_VALIDATION_LAYERS) 
		requiredExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

	return requiredExtensions;
}

bool Instance::checkInstanceExtensionSupport(std::vector<const char*> requiredExtensions)
{
	//----- Check available extensions -----//
	for(auto requiredExtension : requiredExtensions)
	{
		bool hasExtension = false;
		for(auto extension : _extensions)
		{
			if(strcmp(requiredExtension, extension.extensionName))
			{
				hasExtension = true;
				break;
			}
		}
		if(!hasExtension)
			return false;
	}
	return true;
}

void Instance::getVulkanDevices()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		std::cout << BOLDRED << "[Instance] Failed to find GPUs with Vulkan support!";
		exit(1);
	}

	_physicalDevices.resize(deviceCount);
	vkEnumeratePhysicalDevices(_instance, &deviceCount, _physicalDevices.data());
}

void Instance::getVulkanExtensions()
{
	uint32_t extensionCount;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	_extensions.resize(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, _extensions.data());
}
