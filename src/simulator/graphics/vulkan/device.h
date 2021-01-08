//--------------------------------------------------
// Robot Simulator
// device.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef DEVICE_H
#define DEVICE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <memory>
#include "defines.h"
#include "physicalDevice.h"

class Device
{
	public:
		Device(std::shared_ptr<PhysicalDevice> physicalDevice);
		~Device();

		VkDevice handle() const { return _device; }
		std::shared_ptr<PhysicalDevice> getPhysicalDevice() const { return _physicalDevice; }
		VkQueue getGraphicsQueue() const { return _graphicsQueue; }
		VkQueue getPresentQueue() const { return _presentQueue; }

		VkSampleCountFlagBits getMsaaSamples() const { return _msaaSamples; }
		bool getRayTracingEnabled() const { return _rayTracingEnabled; }

	private:
		VkSampleCountFlagBits getMaxUsableSampleCount();

		VkDevice _device;
		VkQueue _graphicsQueue;
		VkQueue _presentQueue;
		std::shared_ptr<PhysicalDevice> _physicalDevice;
		VkSampleCountFlagBits _msaaSamples;

		//---------- Device properties ----------//
		bool _rayTracingEnabled;
};

#endif// DEVICE_H
