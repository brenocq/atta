//--------------------------------------------------
// Robot Simulator
// physicalDevice.cpp
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "physicalDevice.h"
#include "simulator/helpers/log.h"
#include "simulator/graphics/core/window.h"

namespace atta::vk
{
	PhysicalDevice::PhysicalDevice(std::shared_ptr<Instance> instance):
		_instance(instance)
	{
		_physicalDevice = VK_NULL_HANDLE;
		_deviceExtensions = deviceExtensions;

		// Get device count
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(_instance->handle(), &deviceCount, nullptr);
		
		// Check if some device was found
		if(deviceCount == 0) 
		{
			Log::error("PhysicalDevice", "Failed to find GPUs with Vulkan support!");
			exit(1);
		}

		// Get physical devices
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(_instance->handle(), &deviceCount, devices.data());

		printPhysicalDevices(devices);

		// Create temporary surface to select physical device with surface support
		std::shared_ptr<Window> window = std::make_shared<Window>();
		std::shared_ptr<Surface> surface = std::make_shared<Surface>(_instance, window);

		// TODO remake physical device selection
		for(const auto& device : devices) 
		{
			if(isDeviceSuitable(device, surface)) 
			{
				_physicalDevice = device;
				break;
			}
		}

		if(_physicalDevice == VK_NULL_HANDLE)
		{
			Log::warning("PhysicalDevice", "No physical device with ray tracing support, trying without it.");

			for(unsigned int i=0; i<_deviceExtensions.size(); i++)
				if(strcmp(_deviceExtensions[i],VK_NV_RAY_TRACING_EXTENSION_NAME)==0)
				{
					_deviceExtensions.erase(_deviceExtensions.begin()+i);
					break;
				}

			for(const auto& device : devices) 
			{
				if(isDeviceSuitable(device, surface)) 
				{
					_physicalDevice = device;
					break;
				}
			}
		}

		if(_physicalDevice == VK_NULL_HANDLE) 
		{
			Log::error("PhysicalDevice", "Failed to find a suitable GPU!");
			exit(1);
		}
		else
		{
			_queueFamilyIndices = findQueueFamilies(_physicalDevice, surface);

			Log::success("PhysicalDevice", "Found suitable GPU!");
		}
	}

	PhysicalDevice::~PhysicalDevice()
	{
	}

	bool PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device, std::shared_ptr<Surface> surface_) 
	{
		QueueFamilyIndices indices = findQueueFamilies(device, surface_);

		bool extensionsSupported = checkDeviceExtensionSupport(device);

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

		return indices.isComplete() && extensionsSupported /*&& swapChainAdequate */&& supportedFeatures.samplerAnisotropy;
	}

	bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) 
	{
		// Get device extensions
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(_deviceExtensions.begin(), _deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		for(auto extension : requiredExtensions)
			Log::warning("PhysicalDevice", "This device does not support "+std::string(extension));

		return requiredExtensions.empty();
	}

	QueueFamilyIndices PhysicalDevice::findQueueFamilies(VkPhysicalDevice device, std::shared_ptr<Surface> surface_) 
	{
		// Logic to find graphics queue family
		QueueFamilyIndices indices;	

		// Get queue families
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for(const auto& queueFamily : queueFamilies) 
		{
			if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
			{
				indices.graphicsFamily = i;
			}

			// Check queueFamily surface support (present support)
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface_->handle(), &presentSupport);
			if(presentSupport) 
			{
				indices.presentFamily = i;
			}


			if(indices.isComplete()) 
			{
				break;
			}

			i++;
		}

		return indices;
	}

	std::string PhysicalDevice::getVersion(const uint32_t version)
	{
		// Convert version to human readable
		std::stringstream ss;
		ss << VK_VERSION_MAJOR(version);
		ss << ".";
		ss << VK_VERSION_MINOR(version);
		ss << ".";
		ss << VK_VERSION_PATCH(version);

		return ss.str();
	}

	std::string PhysicalDevice::getVersion(const uint32_t version, const uint32_t vendorId)
	{
		// Convert version to human readable
		std::stringstream ss;
		ss << VK_VERSION_MAJOR(version);
		ss << ".";
		ss << (VK_VERSION_MINOR(version) >> (vendorId == 0x10DE ? 2 : 0));
		ss << ".";
		ss << (VK_VERSION_PATCH(version) >> (vendorId == 0x10DE ? 4 : 0));
		return ss.str();
	}

	void PhysicalDevice::printPhysicalDevices(std::vector<VkPhysicalDevice> physicalDevices)
	{
		bool showQueues = true;
		bool showFeatures = false;
		bool showLimits = false;
		bool showAvailableLayers = false;
		bool showAvailableExtensions = false;
		bool showMemory = false;
		bool showRayTracingInfo = true;

		Log::info("PhysicalDevice", "[w]GPU with Vulkan support ($0)", physicalDevices.size());
		for(VkPhysicalDevice device : physicalDevices)
		{
			VkPhysicalDeviceProperties prop;
			vkGetPhysicalDeviceProperties(device, &prop);

			std::string vendorId = "Unknown";
			switch (prop.vendorID)
			{
				case 0x1002:
					vendorId = "AMD";
					break;
				case 0x1010:
					vendorId = "ImgTec";
					break;
				case 0x10DE:
					vendorId = "NVIDIA";
					break;
				case 0x13B5:
					vendorId = "ARM";
					break;
				case 0x5143:
					vendorId = "Qualcomm";
					break;
				case 0x8086:
					vendorId = "Intel";
					break;
				default:
					break;
			}

			std::string deviceType = "Unknown";
			switch (prop.deviceType)
			{
				case VK_PHYSICAL_DEVICE_TYPE_OTHER:
					deviceType = "Other";
					break;
				case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
					deviceType = "Integrated GPU";
					break;
				case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
					deviceType = "Discrete GPU";
					break;
				case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
					deviceType = "Virtual GPU";
					break;
				case VK_PHYSICAL_DEVICE_TYPE_CPU:
					deviceType = "CPU";
				default:
					break;
			}

			std::cout << "\t- ";
			std::cout << BOLDCYAN << vendorId << " " << prop.deviceName << WHITE;
			std::cout << " (";
			std::cout << deviceType << ": ";
			std::cout << "vulkan " << getVersion(prop.apiVersion) << ", ";
			std::cout << "driver " << getVersion(prop.driverVersion, prop.vendorID);
			std::cout << ")" << std::endl;

			if(showQueues)
			{
				uint32_t queueFamilyCount = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
				std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
				vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
				
				std::cout << "\t  - " << CYAN << "QueueFamilies" << WHITE << " (" << queueFamilyCount << "):" << std::endl;
				for(auto queueFamily : queueFamilies)
				{
					std::cout << "\t    - ";
					if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
						std::cout << "G";
					if(queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
						std::cout << "C";
					if(queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
						std::cout << "T";
					if(queueFamily.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
						std::cout << "S";
					if(queueFamily.queueFlags & VK_QUEUE_PROTECTED_BIT)
						std::cout << "P";
					printf(" (%d queues)\n", queueFamily.queueCount);

				}
			}

			if(showLimits)
			{
				VkPhysicalDeviceLimits limits = prop.limits;
				std::cout << "\t  - " << CYAN << "Limits" << WHITE << std::endl;
				std::cout << "\t    - maxImageDimension1D: " << limits.maxImageDimension1D << " texels" << std::endl;
				std::cout << "\t    - maxImageDimension2D: " << limits.maxImageDimension2D << " texels" << std::endl;
				std::cout << "\t    - maxImageDimension3D: " << limits.maxImageDimension3D << " texels" << std::endl;
				std::cout << "\t    - maxTexelBufferElements: " << limits.maxTexelBufferElements << " texels (" 
					<< (int)sqrt(limits.maxTexelBufferElements) << " x " << (int)sqrt(limits.maxTexelBufferElements) << ")" << std::endl;
				std::cout << "\t    - maxMemoryAllocationCount: " << limits.maxMemoryAllocationCount << " times" << std::endl;
			}

			if(showFeatures)
			{
				std::cout << WHITE << "\t  - " << CYAN << "Features" << WHITE << std::endl;

				VkPhysicalDeviceFeatures features;
				vkGetPhysicalDeviceFeatures(device, &features);
				// TODO print features used in this application
				// https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPhysicalDeviceFeatures.html

				std::cout << "\t    - wideLines: " << (features.wideLines? "OK" : "Does not support") << std::endl;
				std::cout << "\t    - largePoints: " << (features.largePoints? "OK" : "Does not support") << std::endl;
				std::cout << "\t    - depthClamp: " << (features.depthClamp? "OK" : "Does not support") << std::endl;
				std::cout << "\t    - compression BC: " << (features.textureCompressionBC? "OK" : "Does not support") << std::endl;
				std::cout << "\t    - compression ETC2: " << (features.textureCompressionETC2? "OK" : "Does not support") << std::endl;
				std::cout << "\t    - compression ASTC_LDR: " << (features.textureCompressionASTC_LDR? "OK" : "Does not support") << std::endl;
			}

			if(showAvailableLayers)
			{
				std::cout << "\t  - " << CYAN << "Available Layers" << WHITE << std::endl;

				uint32_t propertyCount = 0;
				vkEnumerateDeviceLayerProperties(device, &propertyCount, nullptr);

				if(propertyCount == 0)
				{
					std::cout << RED << "\t    There are no available layer properties!" << WHITE << std::endl;
					return;
				}

				std::vector<VkLayerProperties> properties(propertyCount);
				vkEnumerateDeviceLayerProperties(device, &propertyCount, properties.data());

				for (const auto& property : properties) 
				{
					std::cout << "\t    - " << property.layerName << " (" << property.description << ")" << std::endl;
				}

				std::cout << WHITE;
			}

			if(showAvailableExtensions)
			{
				std::cout << "\t  - " << CYAN << "Available extensions" << WHITE << std::endl;

				uint32_t propertyCount = 0;
				vkEnumerateDeviceExtensionProperties(device, nullptr, &propertyCount, nullptr);

				if(propertyCount == 0)
				{
					std::cout << RED << "\t    There are no available extension properties!" << WHITE << std::endl;
					return;
				}

				std::vector<VkExtensionProperties> properties(propertyCount);
				vkEnumerateDeviceExtensionProperties(device, nullptr, &propertyCount, properties.data());

				for (const auto& property : properties) 
				{
					std::cout << "\t    - " << property.extensionName << std::endl;
				}

				std::cout << WHITE;
			}
			
			if(showMemory)
			{
				std::cout << "\t  - " << CYAN << "Memory" << WHITE << std::endl;

				VkPhysicalDeviceMemoryProperties memProp;
				vkGetPhysicalDeviceMemoryProperties(device, &memProp);
				//std::cout << memProp.memoryTypeCount << std::endl;
				std::cout << "\t    - heaps (" << memProp.memoryHeapCount << ")" << std::endl;
				for(int i=0;i<(int)memProp.memoryHeapCount;i++)
					std::cout << "\t      - heap " << i << ": " << -int(memProp.memoryHeaps[i].size)/int(1<<20) << "Mb" << std::endl;
			}

			if(showRayTracingInfo)
			{
				std::cout << "\t  - " << CYAN << "Ray tracing" << WHITE << std::endl;

				VkPhysicalDeviceRayTracingPropertiesNV nvProps = {};
				nvProps.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PROPERTIES_NV;

				VkPhysicalDeviceProperties2 props = {};
				props.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
				props.pNext = &nvProps;
				vkGetPhysicalDeviceProperties2(device, &props);

				std::cout << "\t    - maxRecursionDepth: " << nvProps.maxRecursionDepth << std::endl;
				std::cout << "\t    - shaderGroupBaseAlignment: " << nvProps.shaderGroupBaseAlignment << std::endl;
			}
		}
	}
}
