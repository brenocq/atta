//--------------------------------------------------
// Robot Simulator
// device.cpp
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "device.h"
#include "simulator/helpers/log.h"

namespace atta::vk
{
	Device::Device(std::shared_ptr<PhysicalDevice> physicalDevice):
		_physicalDevice(physicalDevice), _msaaSamples(VK_SAMPLE_COUNT_1_BIT), _rayTracingEnabled(false)
	{
		_msaaSamples = getMaxUsableSampleCount();

		//----- Get queues -----//
		QueueFamilyIndices indices = _physicalDevice->findQueueFamilies();

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

		float queuePriority = 1.0f;
		for(uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.flags = 0;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		//----- Get features -----//
		VkPhysicalDeviceFeatures deviceFeatures{};
		// TODO check for support and store supported features
		deviceFeatures.samplerAnisotropy = VK_TRUE;
		deviceFeatures.fillModeNonSolid = VK_TRUE;
		deviceFeatures.wideLines = VK_TRUE;

		VkPhysicalDeviceDescriptorIndexingFeaturesEXT indexingFeatures = {};
		indexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT;
		indexingFeatures.runtimeDescriptorArray = true;

		//---------- Create logical device ----------//
		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pNext = &indexingFeatures;

		//----- Defines queues -----//
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		//----- Enable features -----//
		createInfo.pEnabledFeatures = &deviceFeatures;

		//----- Enable extensions -----//
		const std::vector<const char*> __deviceExtensions = _physicalDevice->getDeviceExtensions();
		// Check ray tracing enabled
		for(auto ext : __deviceExtensions)
		{
			if(strcmp(ext, VK_NV_RAY_TRACING_EXTENSION_NAME)==0)
				_rayTracingEnabled = true;
		}

		createInfo.enabledExtensionCount = static_cast<uint32_t>(__deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = __deviceExtensions.data();

		//----- Enable layers -----//
		if(ENABLE_VALIDATION_LAYERS) {
			Log::warning("Device", "Validation layers activated.");
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		} else {
			createInfo.enabledLayerCount = 0;
		}

		//----- Create -----//
		if(vkCreateDevice(_physicalDevice->handle(), &createInfo, nullptr, &_device) != VK_SUCCESS)
		{
			Log::error("Device", "Failed to create device.");
			exit(1);
		}
		vkGetDeviceQueue(_device, indices.graphicsFamily.value(), 0, &_graphicsQueue);	
		vkGetDeviceQueue(_device, indices.presentFamily.value(), 0, &_presentQueue);
	}

	Device::~Device()
	{
		vkDeviceWaitIdle(_device);
		if(_device != nullptr)
		{
			vkDestroyDevice(_device, nullptr);
			_device = nullptr;
		}
	}

	VkSampleCountFlagBits Device::getMaxUsableSampleCount()
	{
		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(_physicalDevice->handle(), &physicalDeviceProperties);

		VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
		if(counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
		if(counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
		if(counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
		if(counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
		if(counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
		if(counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

		return VK_SAMPLE_COUNT_1_BIT;
	}
}