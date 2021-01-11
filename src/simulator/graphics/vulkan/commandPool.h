//--------------------------------------------------
// Robot Simulator
// commandPool.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_COMMAND_POOL_H
#define ATTA_VK_COMMAND_POOL_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"

namespace atta::vk
{
	class CommandPool
	{
		public:
			CommandPool(std::shared_ptr<Device> device, VkCommandPoolCreateFlags flags=0);
			~CommandPool();

			VkCommandPool handle() const { return _commandPool; }
			std::shared_ptr<Device> getDevice() const { return _device; }

			VkCommandBuffer beginSingleTimeCommands();
			void endSingleTimeCommands(VkCommandBuffer commandBuffer);
		private:
			VkCommandPool _commandPool;
			std::shared_ptr<Device> _device;
	};
}

#endif// ATTA_VK_COMMAND_POOL_H
