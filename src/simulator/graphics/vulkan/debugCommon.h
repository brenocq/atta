//--------------------------------------------------
// Robot Simulator
// debugCommon.h
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
// The debug common was created because both debugMessenger.cpp and instance.cpp need to access these functions
#ifndef ATTA_VK_DEBUG_COMMON_H
#define ATTA_VK_DEBUG_COMMON_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include "defines.h"

VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
		VkDebugUtilsMessageTypeFlagsEXT messageType, 
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
		void* pUserData);

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

#endif// ATTA_VK_DEBUG_COMMON_H

