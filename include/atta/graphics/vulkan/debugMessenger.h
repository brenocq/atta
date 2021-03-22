//--------------------------------------------------
// Robot Simulator
// debugMessenger.h
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VULKAN_DEBUG_UTILS_MESSENGER_H
#define ATTA_GRAPHICS_VULKAN_DEBUG_UTILS_MESSENGER_H

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <memory>
#include <string.h>
#include <atta/graphics/vulkan/instance.h>
#include <atta/graphics/vulkan/debugCommon.h>

namespace atta::vk
{
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
}

#endif// ATTA_GRAPHICS_VULKAN_DEBUG_UTILS_MESSENGER_H
