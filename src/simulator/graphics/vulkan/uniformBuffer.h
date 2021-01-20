//--------------------------------------------------
// Robot Simulator
// uniformBuffer.h
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_UNIFORM_BUFFER_H
#define ATTA_VK_UNIFORM_BUFFER_H

#include <iostream>
#include <string.h>
#include <vector>
#include "glm.h"
#include "defines.h"
#include "device.h"
#include "buffer.h"
#include "helpers.h"
#include "simulator/math/math.h"

namespace atta::vk
{
	struct UniformBufferObject
	{
		mat4 viewMat;
		mat4 projMat;
		mat4 viewMatInverse;
		mat4 projMatInverse;
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

#endif// ATTA_VK_UNIFORM_BUFFER_H
