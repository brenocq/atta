#ifndef DEVICE_H
#define DEVICE_H

#include "surface.h"

class Device
{
	public:
	Device(VkPhysicalDevice physicalDevice, Surface* surface);
	~Device();

	VkDevice handle() const { return _device; }
	VkPhysicalDevice physicalDevice() const { return _physicalDevice; }
	Surface* surface() const { return _surface; }

	uint32_t graphicsFamilyIndex() const { return _graphicsFamilyIndex; }
	uint32_t computeFamilyIndex() const { return _computeFamilyIndex; }
	uint32_t presentFamilyIndex() const { return _presentFamilyIndex; }
	uint32_t transferFamilyIndex() const { return _transferFamilyIndex; }
	VkQueue graphicsQueue() const { return _graphicsQueue; }
	VkQueue computeQueue() const { return _computeQueue; }
	VkQueue presentQueue() const { return _presentQueue; }
	VkQueue transferQueue() const { return _transferQueue; }

	void waitIdle() const;

	private:
	std::vector<VkQueueFamilyProperties>::const_iterator findQueue(
			const std::vector<VkQueueFamilyProperties>& queueFamilies,
			const std::string& name,
			const VkQueueFlags requiredBits,
			const VkQueueFlags excludedBits);
	void checkDeviceExtensions();

	VkPhysicalDevice _physicalDevice;
	VkDevice _device;
	Surface* _surface;

	std::vector<const char*> _requiredExtensions;

	uint32_t _graphicsFamilyIndex;
	uint32_t _computeFamilyIndex;
	uint32_t _presentFamilyIndex;
	uint32_t _transferFamilyIndex;

	VkQueue _graphicsQueue;
	VkQueue _computeQueue;
	VkQueue _presentQueue;
	VkQueue _transferQueue;
};

#endif// DEVICE_H
