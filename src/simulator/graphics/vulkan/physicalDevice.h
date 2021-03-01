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
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		std::optional<uint32_t> transferFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value();
		}
	};

	struct PhysicalDeviceSupport {
		bool vulkanRayTracing = false;// true -> Can render using vulkan ray tracing
		bool differentQueuesThreadManagerGUI = false;// true -> Thread manager and GUI do not need synchronization (TODO not being used)
		bool samplerAnisotropyFeature;
	};


	class PhysicalDevice
	{
		// Warning starting from 0
		// Errors starting from 1000
		enum PhysicalDeviceSupportInfo {
			WARN_NO_VULKAN_RAY_TRACING_SUPPORT = 0,
			WARN_NO_DEDICATED_TRANSFER_QUEUE_FAMILY,
			WARN_NO_SAMPLER_ANISOTROPY_FEATURE_SUPPORT,
			ERROR_REQUIRED_QUEUE_FAMILIES_NOT_FOUND=1000,
		};

		public:
			PhysicalDevice(std::shared_ptr<Instance> instance);
			~PhysicalDevice();

			VkPhysicalDevice handle() const { return _physicalDevice; }

			//---------- Getters and Setters ----------//
			std::shared_ptr<Instance> getInstance() const { return _instance; }
			QueueFamilyIndices getQueueFamilyIndices() const { return _queueFamilyIndices; }
			PhysicalDeviceSupport getSupport() const { return _support; }

			const std::vector<const char*> getDeviceSupportedExtensions() const { return _deviceSupportedExtensions; }

		private:
			const std::vector<const char*> getDeviceExtensions() const {
				return {
					VK_KHR_SWAPCHAIN_EXTENSION_NAME,
					VK_KHR_SPIRV_1_4_EXTENSION_NAME,
					VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
					VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME,
					VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
					VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME,
					VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
					VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME
				};
			}

			std::vector<PhysicalDeviceSupportInfo> checkDeviceSupport(VkPhysicalDevice device);
			std::vector<PhysicalDeviceSupportInfo> checkDeviceExtensionsSupport(VkPhysicalDevice device);
			std::vector<PhysicalDeviceSupportInfo> checkDeviceFeaturesSupport(VkPhysicalDevice device);
			std::vector<PhysicalDeviceSupportInfo> findQueueFamilies(VkPhysicalDevice device, QueueFamilyIndices& indices);
			void printPhysicalDevices(std::vector<VkPhysicalDevice> physicalDevices);

			// Printing
			std::string getVersion(const uint32_t version);
			std::string getVersion(const uint32_t version, const uint32_t vendorId);

			VkPhysicalDevice _physicalDevice;
			std::shared_ptr<Instance> _instance;
			QueueFamilyIndices _queueFamilyIndices;
			PhysicalDeviceSupport _support;

			// Extensions
			std::vector<const char*> _deviceSupportedExtensions;
	};
}

#endif// ATTA_VK_PHYSICAL_DEVICE_H
