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

			//---------- Getters and Setters ----------//
			std::shared_ptr<Instance> getInstance() const { return _instance; }
			QueueFamilyIndices getQueueFamilyIndices() const { return _queueFamilyIndices; }
			//std::vector<const char*> getDeviceExtensions() const { return _deviceExtensions; }

			const std::vector<const char*> getDeviceExtensions() const {
				return {
					VK_KHR_SWAPCHAIN_EXTENSION_NAME,
					VK_KHR_SPIRV_1_4_EXTENSION_NAME,
					VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME,
					VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
					VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
					VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME,
					VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
					VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME
				};
			}

		private:
			bool isDeviceSuitable(VkPhysicalDevice device, std::shared_ptr<Surface> surface_);
			bool checkDeviceExtensionSupport(VkPhysicalDevice device);
			void printPhysicalDevices(std::vector<VkPhysicalDevice> physicalDevices);
			QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, std::shared_ptr<Surface> surface_);

			
			// Printing
			std::string getVersion(const uint32_t version);
			std::string getVersion(const uint32_t version, const uint32_t vendorId);

			VkPhysicalDevice _physicalDevice;
			std::shared_ptr<Instance> _instance;
			QueueFamilyIndices _queueFamilyIndices;

			// Extensions
			std::vector<const char*> _deviceExtensions;
	};
}

#endif// ATTA_VK_PHYSICAL_DEVICE_H
