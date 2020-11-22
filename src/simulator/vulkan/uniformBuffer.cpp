//--------------------------------------------------
// Robot Simulator
// uniformBuffer.cpp
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "uniformBuffer.h"
#include <cstring>

UniformBuffer::UniformBuffer(Device* device, VkDeviceSize size):
	Buffer(device, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
	UniformBufferObject ubo;
	ubo.modelView = glm::translate(glm::mat4(1), glm::vec3(0, 0, -2));
	ubo.aperture = 0.05f;
	ubo.focusDistance = 2.0f;
	ubo.projection = glm::perspective(glm::radians(45.0f), 1200 / static_cast<float>(900), 1.0f, 1000.0f);
	ubo.projection[1][1] *= -1; // Inverting Y for Vulkan, https://matthewwellings.com/blog/the-new-vulkan-coordinate-system/
	ubo.modelViewInverse = glm::inverse(ubo.modelView);
	ubo.projectionInverse = glm::inverse(ubo.projection);
	ubo.totalNumberOfSamples = 0;
	ubo.numberOfSamples = 10;
	ubo.numberOfBounces = 6;
	ubo.randomSeed = 1;
	ubo.gammaCorrection = true;
	ubo.hasSky = false;

	const auto data = mapMemory(0, sizeof(UniformBufferObject));
	std::memcpy(data, &ubo, sizeof(ubo));
	unmapMemory();
}

UniformBuffer::~UniformBuffer()
{
}

void UniformBuffer::setValue(UniformBufferObject ubo)
{
	const auto data = mapMemory(0, sizeof(UniformBufferObject));
	std::memcpy(data, &ubo, sizeof(ubo));
	unmapMemory();
}
