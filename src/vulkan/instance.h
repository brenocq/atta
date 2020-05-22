#ifndef INSTANCE_H
#define INSTANCE_H

#include <vector>
#include <string.h>
#include "window.h"

class Instance
{
	public:
	Instance(Window* window, const std::vector<const char*> validationLayers);
	~Instance();

	Window* getWindow() const { return _window; }
	const std::vector<const char*> validationLayers() const { return _validationLayers; }
	const std::vector<VkPhysicalDevice> physicalDevices() const { return _physicalDevices; }
	const std::vector<VkExtensionProperties> extensions() const { return _extensions; }
	VkInstance handle() const { return _instance; }
	Window* window() const { return _window; }

	private:
	bool checkVulkanValidationLayerSupport();
	std::vector<const char*> getInstanceRequiredExtensions();
	bool checkInstanceExtensionSupport(std::vector<const char*> requiredExtensions);
	void getVulkanDevices();
	void getVulkanExtensions();

	VkInstance _instance;
	Window* _window;
	const std::vector<const char*> _validationLayers;
	std::vector<VkPhysicalDevice> _physicalDevices;
	std::vector<VkExtensionProperties> _extensions;
};

#endif// INSTANCE_H
