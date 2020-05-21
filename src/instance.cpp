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
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Breno Queiroz Simulator";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_1;

	// Instance info
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = window->getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	if(ENABLE_VALIDATION_LAYERS) 
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	} 
	else 
	{
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	// Create the Vulkan instance
	if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS) {
		std::cout << BOLDRED << "[Instance] Failed to create instance!";
		exit(1);
	}

	getVulkanDevices();
	//getVulkanExtensions();
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

}
