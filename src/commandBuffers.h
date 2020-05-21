#ifndef COMMAND_BUFFERS_H
#define COMMAND_BUFFERS_H

#include "commandPool.h"
#include <vector>

class CommandBuffers
{
	public:
	CommandBuffers(CommandPool* commandPool, uint32_t size);
	~CommandBuffers();
	uint32_t size() const { return static_cast<uint32_t>(_commandBuffers.size()); }
	VkCommandBuffer& operator [] (const size_t i) { return _commandBuffers[i]; }

	VkCommandBuffer begin(size_t i);
	void end(size_t i);

	private:
	const CommandPool* _commandPool;
	std::vector<VkCommandBuffer> _commandBuffers;
};

#endif// COMMAND_BUFFERS_H
