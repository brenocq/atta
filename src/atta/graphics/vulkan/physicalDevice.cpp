//--------------------------------------------------
// Robot Simulator
// physicalDevice.cpp
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/vulkan/physicalDevice.h>
#include <atta/helpers/log.h>
#include <atta/graphics/core/window.h>

namespace atta::vk
{
	PhysicalDevice::PhysicalDevice(std::shared_ptr<Instance> instance):
		_instance(instance)
	{
		_physicalDevice = VK_NULL_HANDLE;

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

		std::vector<PhysicalDeviceSupportInfo> supportInfo;
		for(const auto& device : devices) 
		{
			supportInfo = checkDeviceSupport(device);
			bool suitable = true;
			for(const auto& info : supportInfo)
			{
				// Check if is error
				if(info>=1000)
				{
					suitable = false;
				}
			}

			if(suitable) 
			{
				_physicalDevice = device;
				checkDeviceExtensionsSupport(_physicalDevice);// Call again to update _deviceSupportedExtensions
				break;// Select the first suitable GPU
			}
		}

		if(_physicalDevice == VK_NULL_HANDLE)
		{
			Log::error("PhysicalDevice", "Sorry, no suitable GPU found. Please create an issue at https://github.com/Brenocq/Atta/issues for Atta to support your GPU in the future.");
			exit(0);
		}
		else
		{
			findQueueFamilies(_physicalDevice, _queueFamilyIndices);
			Log::success("PhysicalDevice", "Found suitable GPU! (for now, Atta only uses one GPU)");

			// Warn about support and populate physical device support
			_support.vulkanRayTracing = true;
			_support.differentQueuesThreadManagerGUI = _queueFamilyIndices.graphicsFamily.value() != _queueFamilyIndices.transferFamily.value();
			_support.samplerAnisotropyFeature = true;
			_support.fillModeNonSolidFeature = true;
			for(const auto& info : supportInfo)
			{
				switch(info)
				{
					case WARN_NO_VULKAN_RAY_TRACING_SUPPORT:
						Log::warning("PhysicalDevice", "The selected GPU does not support Vulkan Ray Tracing");
						_support.vulkanRayTracing = false;
						break;
					case WARN_NO_SAMPLER_ANISOTROPY_FEATURE_SUPPORT:
						Log::warning("PhysicalDevice", "The selected GPU does not support sampler anisotropy");
						_support.samplerAnisotropyFeature = false;
					case WARN_NO_FILL_MODE_NON_SOLID_FEATURE_SUPPORT:
						Log::warning("PhysicalDevice", "The selected GPU does not support fill mode non solid");
						_support.fillModeNonSolidFeature = false;
					default:
						break;
				}
			}
		}
	}

	PhysicalDevice::~PhysicalDevice()
	{
	}

	std::vector<PhysicalDevice::PhysicalDeviceSupportInfo> PhysicalDevice::checkDeviceSupport(VkPhysicalDevice device)
	{
		std::vector<PhysicalDeviceSupportInfo> supportInfo;

		QueueFamilyIndices indices;
		std::vector<PhysicalDeviceSupportInfo> queueFamilySupport = findQueueFamilies(device, indices);
		supportInfo.insert(supportInfo.end(), queueFamilySupport.begin(), queueFamilySupport.end());

		std::vector<PhysicalDeviceSupportInfo> extensionSupport = checkDeviceExtensionsSupport(device);
		supportInfo.insert(supportInfo.end(), extensionSupport.begin(), extensionSupport.end());

		std::vector<PhysicalDeviceSupportInfo> featuresSupport = checkDeviceFeaturesSupport(device);
		supportInfo.insert(supportInfo.end(), featuresSupport.begin(), featuresSupport.end());

		return supportInfo;
	}

	std::vector<PhysicalDevice::PhysicalDeviceSupportInfo> PhysicalDevice::checkDeviceExtensionsSupport(VkPhysicalDevice device) 
	{
		std::vector<PhysicalDeviceSupportInfo> supportInfo;

		// Get device extensions
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		const std::vector<const char*> deviceExtensions = getDeviceExtensions();
		_deviceSupportedExtensions = deviceExtensions;
		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		// Erase supported extensions
		for(const auto& extension : availableExtensions)
			requiredExtensions.erase(extension.extensionName);

		// Populate support info and update device supported extensions
		for(const auto& extension : requiredExtensions)
		{
			for(unsigned i=0; i<_deviceSupportedExtensions.size(); i++)
			{
				if(extension == _deviceSupportedExtensions[i])
				{
					_deviceSupportedExtensions.erase(_deviceSupportedExtensions.begin()+i);
					break;
				}
			}
			if(extension == VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME)
			{
				supportInfo.push_back(WARN_NO_VULKAN_RAY_TRACING_SUPPORT);
			}
			//Log::warning("PhysicalDevice", "This device does not support "+std::string(extension));
		}

		return supportInfo;
	}

	std::vector<PhysicalDevice::PhysicalDeviceSupportInfo> PhysicalDevice::checkDeviceFeaturesSupport(VkPhysicalDevice device)
	{
		std::vector<PhysicalDeviceSupportInfo> supportInfo;

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

		if(!supportedFeatures.samplerAnisotropy)
			supportInfo.push_back(WARN_NO_SAMPLER_ANISOTROPY_FEATURE_SUPPORT);

		if(!supportedFeatures.fillModeNonSolid)
			supportInfo.push_back(WARN_NO_FILL_MODE_NON_SOLID_FEATURE_SUPPORT);

		return supportInfo;
	}

	std::vector<PhysicalDevice::PhysicalDeviceSupportInfo> PhysicalDevice::findQueueFamilies(VkPhysicalDevice device, QueueFamilyIndices& indices ) 
	{
		std::vector<PhysicalDeviceSupportInfo> supportInfo;

		// Get queue families
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		// Find queue families
		int i = 0;
		for(const auto& queueFamily : queueFamilies) 
		{
			if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
				indices.graphicsFamily = i;

			// Check queueFamily surface support (present support)
			VkBool32 presentSupport = glfwGetPhysicalDevicePresentationSupport(_instance->handle(), device, i);
			if(presentSupport) 
				indices.presentFamily = i;

			// Find queue family dedicated to tranfer
			if((int)(queueFamily.queueFlags & (VK_QUEUE_GRAPHICS_BIT|VK_QUEUE_TRANSFER_BIT)) == (int)VK_QUEUE_TRANSFER_BIT) 
				indices.transferFamily = i;

			if(indices.isComplete()) 
				break;

			i++;
		}

		// If can't find dedicated tranfer queue family, choose the first that has transfer support
		if(!indices.transferFamily.has_value())
		{
			supportInfo.push_back(WARN_NO_DEDICATED_TRANSFER_QUEUE_FAMILY);

			int i = 0;
			for(const auto& queueFamily : queueFamilies) 
			{
				if(queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) 
				{
					indices.transferFamily = i;
					break;
				}
				i++;
			}
		}

		if(!indices.isComplete())
			supportInfo.push_back(ERROR_REQUIRED_QUEUE_FAMILIES_NOT_FOUND);

		return supportInfo;
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
			std::cout << COLOR_BOLD_CYAN << vendorId << " " << prop.deviceName << COLOR_WHITE;
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
				
				std::cout << "\t  - " << COLOR_CYAN << "QueueFamilies" << COLOR_WHITE << " (" << queueFamilyCount << "):" << std::endl;
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
				std::cout << "\t  - " << COLOR_CYAN << "Limits" << COLOR_WHITE << std::endl;
				std::cout << "\t    - maxImageDimension1D: " << limits.maxImageDimension1D << " texels" << std::endl;
				std::cout << "\t    - maxImageDimension2D: " << limits.maxImageDimension2D << " texels" << std::endl;
				std::cout << "\t    - maxImageDimension3D: " << limits.maxImageDimension3D << " texels" << std::endl;
				std::cout << "\t    - maxTexelBufferElements: " << limits.maxTexelBufferElements << " texels (" 
					<< (int)sqrt(limits.maxTexelBufferElements) << " x " << (int)sqrt(limits.maxTexelBufferElements) << ")" << std::endl;
				std::cout << "\t    - maxMemoryAllocationCount: " << limits.maxMemoryAllocationCount << " times" << std::endl;
			}

			if(showFeatures)
			{
				std::cout << COLOR_WHITE << "\t  - " << COLOR_CYAN << "Features" << COLOR_WHITE << std::endl;

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
				std::cout << "\t  - " << COLOR_CYAN << "Available Layers" << COLOR_WHITE << std::endl;

				uint32_t propertyCount = 0;
				vkEnumerateDeviceLayerProperties(device, &propertyCount, nullptr);

				if(propertyCount == 0)
				{
					std::cout << COLOR_RED << "\t    There are no available layer properties!" << COLOR_WHITE << std::endl;
					return;
				}

				std::vector<VkLayerProperties> properties(propertyCount);
				vkEnumerateDeviceLayerProperties(device, &propertyCount, properties.data());

				for (const auto& property : properties) 
				{
					std::cout << "\t    - " << property.layerName << " (" << property.description << ")" << std::endl;
				}

				std::cout << COLOR_WHITE;
			}

			if(showAvailableExtensions)
			{
				std::cout << "\t  - " << COLOR_CYAN << "Available extensions" << COLOR_WHITE << std::endl;

				uint32_t propertyCount = 0;
				vkEnumerateDeviceExtensionProperties(device, nullptr, &propertyCount, nullptr);

				if(propertyCount == 0)
				{
					std::cout << COLOR_RED << "\t    There are no available extension properties!" << COLOR_WHITE << std::endl;
					return;
				}

				std::vector<VkExtensionProperties> properties(propertyCount);
				vkEnumerateDeviceExtensionProperties(device, nullptr, &propertyCount, properties.data());

				for (const auto& property : properties) 
				{
					std::cout << "\t    - " << property.extensionName << std::endl;
				}

				std::cout << COLOR_WHITE;
			}
			
			if(showMemory)
			{
				std::cout << "\t  - " << COLOR_CYAN << "Memory" << COLOR_WHITE << std::endl;

				VkPhysicalDeviceMemoryProperties memProp;
				vkGetPhysicalDeviceMemoryProperties(device, &memProp);
				//std::cout << memProp.memoryTypeCount << std::endl;
				std::cout << "\t    - heaps (" << memProp.memoryHeapCount << ")" << std::endl;
				for(int i=0;i<(int)memProp.memoryHeapCount;i++)
					std::cout << "\t      - heap " << i << ": " << -int(memProp.memoryHeaps[i].size)/int(1<<20) << "Mb" << std::endl;
			}

			if(showRayTracingInfo)
			{
				std::cout << "\t  - " << COLOR_CYAN << "Ray tracing KHR" << COLOR_WHITE << std::endl;

				VkPhysicalDeviceRayTracingPipelinePropertiesKHR rtProps = {};
				rtProps.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR;

				VkPhysicalDeviceProperties2 props = {};
				props.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
				props.pNext = &rtProps;
				vkGetPhysicalDeviceProperties2(device, &props);

				std::cout << "\t    - maxRecursionDepth: " << rtProps.maxRayRecursionDepth << std::endl;
				std::cout << "\t    - shaderGroupBaseAlignment: " << rtProps.shaderGroupBaseAlignment << std::endl;
			}
		}
	}
}
