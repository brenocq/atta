#ifndef DEBUG_UTILS_MESSENGER_H
#define DEBUG_UTILS_MESSENGER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "defines.h"

class DebugMessenger
{
	public:
	DebugMessenger();
	~DebugMessenger();

	private:
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, 
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
			const VkAllocationCallbacks* pAllocator, 
			VkDebugUtilsMessengerEXT* pDebugMessenger);

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, 
			VkDebugUtilsMessengerEXT debugMessenger, 
			const VkAllocationCallbacks* pAllocator);

    VkDebugUtilsMessengerEXT _debugMessenger;
};

#endif// DEBUG_UTILS_MESSENGER_H
