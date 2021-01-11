//--------------------------------------------------
// Robot Simulator
// physicalDevice.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_PHYSICAL_DEVICE_H
#define ATTA_VK_PHYSICAL_DEVICE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <set>
#include <optional>
#include <memory>
#include <sstream>
#include <string.h>
#include "defines.h"
#include "instance.h"
#include "surface.h"
#include "helpers.h"

namespace atta::vk
{
	class PhysicalDevice
	{
		public:
			PhysicalDevice(std::shared_ptr<Instance> instance);
			~PhysicalDevice();

			VkPhysicalDevice handle() const { return _physicalDevice; }
			QueueFamilyIndices findQueueFamilies();
			SwapChainSupportDetails querySwapChainSupport();

			//---------- Getters and Setters ----------//
			std::shared_ptr<Instance> getInstance() const { return _instance; }
			//std::vector<const char*> getDeviceExtensions() const { return _deviceExtensions; }

			const std::vector<const char*> getDeviceExtensions() const {
				return {
					VK_KHR_SWAPCHAIN_EXTENSION_NAME
				};
			}

		private:
			bool isDeviceSuitable(VkPhysicalDevice device);
			bool checkDeviceExtensionSupport(VkPhysicalDevice device);
			SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
			void printPhysicalDevices(std::vector<VkPhysicalDevice> physicalDevices);
			QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
			
			// Printing
			std::string getVersion(const uint32_t version);
			std::string getVersion(const uint32_t version, const uint32_t vendorId);

			VkPhysicalDevice _physicalDevice;
			std::shared_ptr<Instance> _instance;

			// Extensions
			std::vector<const char*> _deviceExtensions;
	};
}

#endif// ATTA_VK_PHYSICAL_DEVICE_H
