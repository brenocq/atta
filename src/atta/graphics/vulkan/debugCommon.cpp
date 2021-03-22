//--------------------------------------------------
// Robot Simulator
// debugCommon.cpp
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/vulkan/debugCommon.h>
#include <atta/helpers/log.h>
#include <vector>

const char* objectTypeToString(const VkObjectType objectType)
{
	// Thanks to https://github.com/GPSnoopy/RayTracingInVulkan
	switch (objectType)
	{
#define STR(e) case VK_OBJECT_TYPE_ ## e: return # e
	STR(UNKNOWN);
	STR(INSTANCE);
	STR(PHYSICAL_DEVICE);
	STR(DEVICE);
	STR(QUEUE);
	STR(SEMAPHORE);
	STR(COMMAND_BUFFER);
	STR(FENCE);
	STR(DEVICE_MEMORY);
	STR(BUFFER);
	STR(IMAGE);
	STR(EVENT);
	STR(QUERY_POOL);
	STR(BUFFER_VIEW);
	STR(IMAGE_VIEW);
	STR(SHADER_MODULE);
	STR(PIPELINE_CACHE);
	STR(PIPELINE_LAYOUT);
	STR(RENDER_PASS);
	STR(PIPELINE);
	STR(DESCRIPTOR_SET_LAYOUT);
	STR(SAMPLER);
	STR(DESCRIPTOR_POOL);
	STR(DESCRIPTOR_SET);
	STR(FRAMEBUFFER);
	STR(COMMAND_POOL);
	STR(SAMPLER_YCBCR_CONVERSION);
	STR(DESCRIPTOR_UPDATE_TEMPLATE);
	STR(SURFACE_KHR);
	STR(SWAPCHAIN_KHR);
	STR(DISPLAY_KHR);
	STR(DISPLAY_MODE_KHR);
	STR(DEBUG_REPORT_CALLBACK_EXT);
	STR(DEBUG_UTILS_MESSENGER_EXT);
	STR(ACCELERATION_STRUCTURE_KHR);
	STR(VALIDATION_CACHE_EXT);
	STR(PERFORMANCE_CONFIGURATION_INTEL);
	STR(DEFERRED_OPERATION_KHR);
	STR(INDIRECT_COMMANDS_LAYOUT_NV);
#undef STR
	default: return "unknown";
	}
}


VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
		VkDebugUtilsMessageTypeFlagsEXT messageType, 
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
		void* pUserData)
{
	if(pCallbackData->objectCount == 0 || messageSeverity <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
		return VK_FALSE;

	//---------- Ignore some false alarm messages
	std::vector<std::string> messagesToIgnore = {"/usr/lib/i386-linux-gnu/"};

	for (uint32_t i = 0; i != pCallbackData->objectCount; ++i)
	{
		std::string message = pCallbackData->pMessage;

		// Search if some message matches with a message to ignore
		for(std::string messageToIgnore : messagesToIgnore)
		{
			if(message.find(messageToIgnore) != std::string::npos)
			{
				return VK_FALSE;
			}
		}
	}

	//---------- Print messages
	std::cerr << std::endl;
	switch (messageSeverity)
	{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			std::cerr << COLOR_WHITE << "[Debug Messenger] Verbose";
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			std::cerr << COLOR_BLUE << "[Debug Messenger] Info";
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			std::cerr << COLOR_BOLD_YELLOW << "[Debug Messenger] Warning";
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			std::cerr << COLOR_BOLD_RED << "[Debug Messenger] Error";
			break;
		default:;
			std::cerr << COLOR_BOLD_RED << "[Debug Messenger] Unknown";
	}

	switch (messageType)
	{
		case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
			std::cerr << "(General): " << COLOR_WHITE;
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
			std::cerr << "(Validation): " << COLOR_WHITE;
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
			std::cerr << "(Performace): " << COLOR_WHITE;
			break;
		default:
			std::cerr << "(Unknown): " << COLOR_WHITE;
	}

	std::cerr << "\n    Objects (" << pCallbackData->objectCount << "):\n";
	for (uint32_t i = 0; i != pCallbackData->objectCount; ++i)
	{
		const auto object = pCallbackData->pObjects[i];
		std::cerr
			<< "    - Object[" << i << "]: " <<
			COLOR_RED << "\n      Type: " << COLOR_WHITE << objectTypeToString(object.objectType ) <<
			COLOR_RED << "\n      Handle: " << COLOR_WHITE <<reinterpret_cast<void*>(object.objectHandle) <<
			COLOR_RED << "\n      Name: " << COLOR_WHITE << "'" << (object.pObjectName ? object.pObjectName : "") << "'"
			COLOR_RED << "\n      Message: " << COLOR_WHITE << pCallbackData->pMessage << 
			COLOR_RESET << std::endl;
	}

	return VK_FALSE;
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) 
{
	// Debug messenger
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = vulkanDebugCallback;

	// Enable best practices
	//VkValidationFeatureEnableEXT enables[] = {VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT};
	//VkValidationFeaturesEXT features = {};
	//features.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
	//features.enabledValidationFeatureCount = 1;
	//features.pEnabledValidationFeatures = enables;

	////features.enabledValidationFeatureCount = 0;
	////features.pEnabledValidationFeatures = nullptr;

	//createInfo.pNext = &features;
}
