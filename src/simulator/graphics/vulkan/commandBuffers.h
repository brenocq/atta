//--------------------------------------------------
// Robot Simulator
// commandBuffers.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef COMMAND_BUFFERS_H
#define COMMAND_BUFFERS_H

#include <iostream>
#include <string.h>
#include <vector>
#include "defines.h"
#include "device.h"
#include "commandPool.h"
#include "frameBuffer.h"

class CommandBuffers
{
	public:
		CommandBuffers(std::shared_ptr<Device> device, std::shared_ptr<CommandPool> commandPool, uint32_t size);
		~CommandBuffers();

		std::vector<VkCommandBuffer> handle() const { return _commandBuffers; }
		void setHandle(std::vector<VkCommandBuffer> newCommandBuffers) { _commandBuffers = newCommandBuffers; }
		std::shared_ptr<Device> getDevice() const { return _device; }

		VkCommandBuffer begin(size_t i);
		void end(size_t i);

	private:
		std::shared_ptr<Device> _device;
		std::vector<VkCommandBuffer> _commandBuffers;
		std::shared_ptr<CommandPool> _commandPool;
};

#endif// COMMAND_BUFFERS_H
