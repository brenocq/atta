//--------------------------------------------------
// Robot Simulator
// guiUniformBuffer.cpp
// Date: 2020-11-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "guiUniformBuffer.h"
#include <cstring>

GuiUniformBuffer::GuiUniformBuffer(std::shared_ptr<Device> device, VkDeviceSize size):
	Buffer(device, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
	GuiUniformBufferObject ubo;
	ubo.ratio = 1;
	ubo.debug = false;

	const auto data = mapMemory(0, sizeof(GuiUniformBufferObject));
	std::memcpy(data, &ubo, sizeof(ubo));
	unmapMemory();
}

GuiUniformBuffer::~GuiUniformBuffer()
{
}

void GuiUniformBuffer::setValue(GuiUniformBufferObject ubo)
{
	const auto data = mapMemory(0, sizeof(GuiUniformBufferObject));
	std::memcpy(data, &ubo, sizeof(ubo));
	unmapMemory();
}
