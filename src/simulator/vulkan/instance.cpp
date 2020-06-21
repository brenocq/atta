#include "instance.h"

const std::vector<const char*> _validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

Instance::Instance()
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
    appInfo.pApplicationName = "Robot Simulator";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;	

	// Create instance info
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	// Get glfw extensions
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (ENABLE_VALIDATION_LAYERS) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
		createInfo.ppEnabledLayerNames = _validationLayers.data();
	} else {
		createInfo.enabledLayerCount = 0;
	}

	printExtensionSupport();

	if(vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[Instance]" << RESET << RED << " Failed to create vulkan instance!" << RESET << std::endl;
		exit(1);
	}
}

Instance::~Instance()
{
	vkDestroyInstance(_instance, nullptr);
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

bool Instance::checkValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// Check if all validation layers are in the available layers
	for(const char* layerName : _validationLayers) 
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
