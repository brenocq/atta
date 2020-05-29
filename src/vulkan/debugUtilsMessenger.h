#ifndef DEBUG_UTILS_MESSENGER_H
#define DEBUG_UTILS_MESSENGER_H

#include "instance.h"

class DebugUtilsMessenger
{
public:
	DebugUtilsMessenger(Instance* instance, VkDebugUtilsMessageSeverityFlagBitsEXT threshold);
	~DebugUtilsMessenger();

	VkDebugUtilsMessengerEXT handle() const { return _messenger; }
	VkDebugUtilsMessageSeverityFlagBitsEXT threshold() const { return _threshold; }

private:

	Instance* _instance;
	const VkDebugUtilsMessageSeverityFlagBitsEXT _threshold;

	VkDebugUtilsMessengerEXT _messenger;
};

#endif// DEBUG_UTILS_MESSENGER_H
