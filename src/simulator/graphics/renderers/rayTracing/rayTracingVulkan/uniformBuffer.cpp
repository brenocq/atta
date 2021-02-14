//--------------------------------------------------
// Atta Ray Tracing Vulkan
// uniformBuffer.cpp
// Date: 2021-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "uniformBuffer.h"
#include <cstring>
#include <iostream>

namespace atta::rt::vk
{
	UniformBuffer::UniformBuffer(std::shared_ptr<atta::vk::Device> device):
		Buffer(device, sizeof(UniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
	{
	}

	UniformBuffer::~UniformBuffer()
	{
	}

	void UniformBuffer::setValue(UniformBufferObject ubo)
	{
		_ubo = ubo;
		const auto data = mapMemory(0, sizeof(UniformBufferObject));
		std::memcpy(data, &ubo, sizeof(ubo));
		unmapMemory();
	}
}
