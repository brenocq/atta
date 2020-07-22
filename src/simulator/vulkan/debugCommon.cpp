//--------------------------------------------------
// Robot Simulator
// debugCommon.cpp
// Date: 21/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "debugCommon.h"

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
			std::cerr << WHITE << "[Debug Messenger] Verbose";
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			std::cerr << BLUE << "[Debug Messenger] Info";
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			std::cerr << BOLDYELLOW << "[Debug Messenger] Warning";
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			std::cerr << BOLDRED << "[Debug Messenger] Error";
			break;
		default:;
			std::cerr << BOLDRED << "[Debug Messenger] Unknown";
	}

	switch (messageType)
	{
		case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
			std::cerr << "(General): " << WHITE;
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
			std::cerr << "(Validation): " << WHITE;
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
			std::cerr << "(Performace): " << WHITE;
			break;
		default:
			std::cerr << "(Unknown): " << WHITE;
	}

	std::cerr << "\n    Objects (" << pCallbackData->objectCount << "):\n";
	for (uint32_t i = 0; i != pCallbackData->objectCount; ++i)
	{
		const auto object = pCallbackData->pObjects[i];
		std::cerr
			<< "    - Object[" << i << "]: " <<
			RED << "\n      Type: " << WHITE << objectTypeToString(object.objectType ) <<
			RED << "\n      Handle: " << WHITE <<reinterpret_cast<void*>(object.objectHandle) <<
			RED << "\n      Name: " << WHITE << "'" << (object.pObjectName ? object.pObjectName : "") << "'"
			RED << "\n      Message: " << WHITE << pCallbackData->pMessage << 
			RESET << std::endl;
	}

	return VK_FALSE;
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) 
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = vulkanDebugCallback;
}
