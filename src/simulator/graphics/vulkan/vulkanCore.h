//--------------------------------------------------
// Atta Graphics
// vulkanCore.h
// Date: 2021-01-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VULKAN_CORE_H
#define ATTA_GRAPHICS_VULKAN_CORE_H

#include <vector>
#include <memory>
#include "simulator/graphics/vulkan/instance.h"
#include "simulator/graphics/vulkan/debugMessenger.h"
#include "simulator/graphics/vulkan/physicalDevice.h"
#include "simulator/graphics/vulkan/device.h"

namespace atta::vk
{
	class VulkanCore
	{
		public:
			VulkanCore();
			~VulkanCore();

		private:
			std::shared_ptr<Instance> _instance;
			std::unique_ptr<DebugMessenger> _debugMessenger;
			std::shared_ptr<PhysicalDevice> _physicalDevice;
			std::shared_ptr<Device> _device;
	};
}

#endif// ATTA_GRAPHICS_VULKAN_CORE_H
