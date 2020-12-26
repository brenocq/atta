//--------------------------------------------------
// Robot Simulator
// instance.cpp
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "instance.h"
#include "simulator/helpers/log.h"

//--------------------- Instance class -------------------//
Instance::Instance():
	_instance(VK_NULL_HANDLE)
{
	// Check validation layers support if requested
	if(ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport())
	{
		Log::error("Instance", "Validation layers requested, but not available!");
		exit(1);
	}

	// Application info
	VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Robot Simulator - by Brenocq";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

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

		// Best practices
		//VkValidationFeatureEnableEXT enables[] = {VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT};
		//VkValidationFeaturesEXT features = {};
		//features.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
		//features.enabledValidationFeatureCount = 1;
		//features.pEnabledValidationFeatures = enables;

		//// Next
		//features.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
		//createInfo.pNext = &features;

		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
	} else {
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	//printExtensionSupport();
	//printLayersProperties();
	if(vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS)
	{
		Log::error("Instance", "Failed to create vulkan instance!");
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
	Log::info("Instance", "Available instance extensions:");

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	for(const auto& extension : extensions) 
	{
		Log::infoItem("Instance", extension.extensionName);
	}

	std::cout << RESET;
}

void Instance::printLayersProperties()
{
	Log::info("Instance", "Available instance layers:");

	uint32_t propertyCount = 0;
	vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);

	if(propertyCount == 0)
	{
		Log::warning("Instance", "There are no available layer properties!", false);
		return;
	}

	std::vector<VkLayerProperties> properties(propertyCount);
	vkEnumerateInstanceLayerProperties(&propertyCount, properties.data());

	for(const auto& property : properties) 
	{
		Log::infoItem("Instance", property.layerName+std::string(" (")+property.description+std::string(")"));
	}

	std::cout << RESET;
}

std::vector<const char*> Instance::getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if(ENABLE_VALIDATION_LAYERS) {
		//extensions.push_back(VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME);
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	for(auto extension : instanceExtensions)
	{
		extensions.push_back(extension);
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
