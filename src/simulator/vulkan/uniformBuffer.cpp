//--------------------------------------------------
// Robot Simulator
// uniformBuffer.cpp
// Date: 06/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "uniformBuffer.h"

UniformBuffer::UniformBuffer(Device* device, VkDeviceSize size):
	Buffer(device, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
}

UniformBuffer::~UniformBuffer()
{
}
