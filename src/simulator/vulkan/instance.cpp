//--------------------------------------------------
// Robot Simulator
// instance.cpp
// Date: 21/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "instance.h"

//--------------------- Instance class -------------------//
Instance::Instance():
	_instance(VK_NULL_HANDLE)
{
	// Check validation layers support if requested
	if(ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport())
	{
		std::cout << BOLDRED << "[Instance]" << RESET << RED << " Validation layers requested, but not available!" << RESET << std::endl;
		exit(1);
	}

	// Application info
	VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Robot Simulator - by Brenocq";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;	

	// Create instance info
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	// Setup required extensions
	std::vector<const char*> extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	// Setup validation layers
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
	if (ENABLE_VALIDATION_LAYERS) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		// Debug debug utils messenger
		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
	} else {
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	//printExtensionSupport();
	if(vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[Instance]" << RESET << RED << " Failed to create vulkan instance!" << RESET << std::endl;
		exit(1);
	}
}

Instance::~Instance()
{
	vkDestroyInstance(_instance, nullptr);
	_instance = nullptr;
}

void Instance::printExtensionSupport()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	std::cout << BOLDWHITE << "[Instance]" << WHITE <<" Available vulkan extensions:\n";
	for (const auto& extension : extensions) 
	{
		std::cout << '\t' << extension.extensionName << '\n';
	}

	std::cout << RESET;
}

std::vector<const char*> Instance::getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (ENABLE_VALIDATION_LAYERS) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

bool Instance::checkValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// Check if all validation layers are in the available layers
	for(const char* layerName : validationLayers) 
	{
		bool layerFound = false;

		for(const auto& layerProperties : availableLayers) 
		{
			if(strcmp(layerName, layerProperties.layerName) == 0) 
			{
				layerFound = true;
				break;
			}
		}

		if(!layerFound) 
		{
			return false;
		}
	}
	return true;
}
