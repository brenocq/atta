#ifndef PHYSICAL_DEVICE_H
#define PHYSICAL_DEVICE_H

#include <string>
#include <sstream>
#include "instance.h"

class PhysicalDevice
{
	public:
		PhysicalDevice(Instance* instance);
		~PhysicalDevice();

		VkPhysicalDevice handle() const { return _physicalDevice; }
		void printPhysicalDevices();

	private:
		VkPhysicalDevice choosePhysicalDevice();
		std::string getVersion(const uint32_t version);
		std::string getVersion(const uint32_t version, const uint32_t vendorId);
		Instance* _instance;
		VkPhysicalDevice _physicalDevice;
};

#endif// PHYSICAL_DEVICE_H
