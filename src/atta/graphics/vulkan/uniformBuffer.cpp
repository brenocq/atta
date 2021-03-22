//--------------------------------------------------
// Robot Simulator
// uniformBuffer.cpp
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/vulkan/uniformBuffer.h>
#include <cstring>
#include <iostream>

namespace atta::vk
{
	UniformBuffer::UniformBuffer(std::shared_ptr<Device> device):
		Buffer(device, sizeof(UniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
	{
		UniformBufferObject ubo;
		ubo.viewMat = mat4(1).translate(vec3(0, 0, -2));
		ubo.projMat = atta::perspective(atta::radians(45.0f), 1200 / static_cast<float>(900), 1.0f, 1000.0f);
		ubo.projMat.data[5] *= -1; // Inverting Y for Vulkan, https://matthewwellings.com/blog/the-new-vulkan-coordinate-system/
		ubo.viewMatInverse = atta::inverse(ubo.viewMat);
		ubo.projMatInverse = atta::inverse(ubo.projMat);

		setValue(ubo);
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
