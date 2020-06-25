//--------------------------------------------------
// Robot Simulator
// commandBuffers.h
// Date: 24/06/2020
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
	CommandBuffers(Device* device, CommandPool* commandPool, std::vector<FrameBuffer*> frameBuffers);
	~CommandBuffers();

	std::vector<VkCommandBuffer> handle() const { return _commandBuffers; }
	void setHandle(std::vector<VkCommandBuffer> newCommandBuffers) { _commandBuffers = newCommandBuffers; }
	Device* getDevice() const { return _device; }

	VkCommandBuffer begin(size_t i);
	void end(size_t i);

	private:
	std::vector<VkCommandBuffer> _commandBuffers;
	Device* _device;
	CommandPool* _commandPool;
};

#endif// COMMAND_BUFFERS_H
