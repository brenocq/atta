//--------------------------------------------------
// Robot Simulator
// commandPool.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VULKAN_COMMAND_POOL_H
#define ATTA_GRAPHICS_VULKAN_COMMAND_POOL_H

#include <iostream>
#include <string.h>
#include <atta/graphics/vulkan/device.h>

namespace atta::vk
{
	class CommandPool
	{
		public:
			enum DeviceQueueFamily {
				DEVICE_QUEUE_FAMILY_GRAPHICS=0,
				DEVICE_QUEUE_FAMILY_TRANSFER,
			};

			enum SubmitQueueType {
				QUEUE_THREAD_MANAGER,
				QUEUE_GUI,
			};

			CommandPool(
					std::shared_ptr<Device> device, 
					DeviceQueueFamily deviceQueueFamily=DEVICE_QUEUE_FAMILY_GRAPHICS, 
					SubmitQueueType submitQueueType=QUEUE_THREAD_MANAGER,
					VkCommandPoolCreateFlags flags=0);
			~CommandPool();

			VkCommandPool handle() const { return _commandPool; }
			std::shared_ptr<Device> getDevice() const { return _device; }

			VkCommandBuffer beginSingleTimeCommands();
			void endSingleTimeCommands(VkCommandBuffer commandBuffer);
			void waitCompletion();

		private:
			VkCommandPool _commandPool;
			std::shared_ptr<Device> _device;
			DeviceQueueFamily _deviceQueueFamily;
			SubmitQueueType _submitQueueType;
			VkQueue _submitQueue;
	};
}

#endif// ATTA_GRAPHICS_VULKAN_COMMAND_POOL_H
