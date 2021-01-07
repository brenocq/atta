//--------------------------------------------------
// Robot Simulator
// debugMessenger.h
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef DEBUG_UTILS_MESSENGER_H
#define DEBUG_UTILS_MESSENGER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <memory>
#include <string.h>
#include "defines.h"
#include "instance.h"
#include "debugCommon.h"

class DebugMessenger
{
	public:
		DebugMessenger(std::shared_ptr<Instance> instance);
		~DebugMessenger();

	private:
		VkResult CreateDebugUtilsMessengerEXT(
				VkInstance instance,
				const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
				const VkAllocationCallbacks* pAllocator,
				VkDebugUtilsMessengerEXT* pDebugMessenger);

		void DestroyDebugUtilsMessengerEXT(
				VkInstance instance,
				VkDebugUtilsMessengerEXT debugMessenger,
				const VkAllocationCallbacks* pAllocator);

		VkDebugUtilsMessengerEXT _debugMessenger;
		std::shared_ptr<Instance> _instance;
};

#endif// DEBUG_UTILS_MESSENGER_H
