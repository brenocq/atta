//--------------------------------------------------
// Robot Simulator
// uniformBuffer.h
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VULKAN_UNIFORM_BUFFER_H
#define ATTA_GRAPHICS_VULKAN_UNIFORM_BUFFER_H

#include <iostream>
#include <string.h>
#include <vector>
#include <atta/graphics/vulkan/device.h>
#include <atta/graphics/vulkan/buffer.h>
#include <atta/math/math.h>

namespace atta::vk
{
	struct UniformBufferObject
	{
		alignas(16) mat4 viewMat;
		alignas(16) mat4 projMat;
		alignas(16) mat4 viewMatInverse;
		alignas(16) mat4 projMatInverse;

		alignas(4) int nLights;
	};


	class UniformBuffer : public Buffer
	{
		public:
			UniformBuffer(std::shared_ptr<Device> device);
			~UniformBuffer();

			void setValue(UniformBufferObject ubo);
			UniformBufferObject getValue() const { return _ubo; }
			
		private:
			UniformBufferObject _ubo;
	};
}

#endif// ATTA_GRAPHICS_VULKAN_UNIFORM_BUFFER_H
