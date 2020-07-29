//--------------------------------------------------
// Robot Simulator
// uniformBuffer.h
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef UNIFORM_BUFFER_H
#define UNIFORM_BUFFER_H

#include <iostream>
#include <string.h>
#include <vector>
#include "glm.h"
#include "defines.h"
#include "device.h"
#include "buffer.h"
#include "helpers.h"

class UniformBuffer : public Buffer
{
	public:
		UniformBuffer(Device* device, VkDeviceSize size);
		~UniformBuffer();

		void setValue(UniformBufferObject ubo);
	private:
};

#endif// UNIFORM_BUFFER_H
