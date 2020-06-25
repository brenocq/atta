//--------------------------------------------------
// Robot Simulator
// debugMessenger.cpp
// Date: 21/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "debugMessenger.h"


//--------------------- debug helpers -------------------//
VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) 
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) 
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) 
	{
        func(instance, debugMessenger, pAllocator);
    }
} 

//------------------------ Debug messenger class -------------------------//

DebugMessenger::DebugMessenger(Instance* instance)
{
	checkArguments(instance);
	_instance = instance;
	if(!ENABLE_VALIDATION_LAYERS)
		return;

	// Debug messenger create info
	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	populateDebugMessengerCreateInfo(createInfo);

	// Create debug messenger
	if(createDebugUtilsMessengerEXT(_instance->handle(), &createInfo, nullptr, &_debugMessenger) != VK_SUCCESS) 
	{
		std::cout << BOLDRED << "[Debug Messenger]" << RESET << RED << " Failed to create vulkan debug utils messenger!" << RESET << std::endl;
		exit(1);
	}
}

DebugMessenger::~DebugMessenger()
{
	if(_debugMessenger != nullptr)
	{
		destroyDebugUtilsMessengerEXT(_instance->handle(), _debugMessenger, nullptr);
	}
}

void DebugMessenger::checkArguments(Instance* instance)
{
	if(instance != nullptr)
	{
		if(instance->handle() == VK_NULL_HANDLE)
		{
			std::cout << BOLDRED << "[Debug Messenger]" << RESET << RED << " Instance handle can't be null!" << RESET << std::endl;
			exit(1);
		}
	}
	else
	{
		std::cout << BOLDRED << "[Debug Meesenger]" << RESET << RED << " Instance can't be null!" << RESET << std::endl;
		exit(1);
	}

}
