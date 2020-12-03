//--------------------------------------------------
// Robot Simulator
// physicalDevice.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef PHYSICAL_DEVICE_H
#define PHYSICAL_DEVICE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <set>
#include <optional>
#include <sstream>
#include <string.h>
#include "defines.h"
#include "instance.h"
#include "surface.h"
#include "helpers.h"


class PhysicalDevice
{
	public:
		PhysicalDevice(Instance* instance, Surface* surface);
		~PhysicalDevice();

		VkPhysicalDevice handle() const { return _physicalDevice; }
		QueueFamilyIndices findQueueFamilies();
		SwapChainSupportDetails querySwapChainSupport();

		//---------- Getters and Setters ----------//
		Instance* getInstance() const { return _instance; }
		Surface* getSurface() const { return _surface; }
		//std::vector<const char*> getDeviceExtensions() const { return _deviceExtensions; }

		const std::vector<const char*> getDeviceExtensions() const {
			return {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
			};
		}

	private:
		void checkArguments(Instance* instance, Surface* surface);
		bool isDeviceSuitable(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		void printPhysicalDevices(std::vector<VkPhysicalDevice> physicalDevices);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		
		// Printing
		std::string getVersion(const uint32_t version);
		std::string getVersion(const uint32_t version, const uint32_t vendorId);

		VkPhysicalDevice _physicalDevice;
		Instance* _instance;
		Surface* _surface;

		// Extensions
		std::vector<const char*> _deviceExtensions;
};

#endif// PHYSICAL_DEVICE_H
