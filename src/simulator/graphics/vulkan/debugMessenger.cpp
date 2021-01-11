//--------------------------------------------------
// Robot Simulator
// debugMessenger.cpp
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "debugMessenger.h"
#include "simulator/helpers/log.h"


//--------------------- Debug helpers -------------------//
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

//------------------------ Debug Messenger class -------------------------//

namespace atta::vk
{
	DebugMessenger::DebugMessenger(std::shared_ptr<Instance> instance):
		_instance(instance)
	{
		if(!ENABLE_VALIDATION_LAYERS)
			return;

		// Debug messenger create info
		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		populateDebugMessengerCreateInfo(createInfo);

		// Create debug messenger
		if(createDebugUtilsMessengerEXT(_instance->handle(), &createInfo, nullptr, &_debugMessenger) != VK_SUCCESS) 
		{
			Log::error("Debug Messenger", "Failed to create vulkan debug utils messenger!");
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
}
