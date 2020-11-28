//--------------------------------------------------
// Robot Simulator
// guiUniformBuffer.h
// Date: 2020-11-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUI_UNIFORM_BUFFER_H
#define GUI_UNIFORM_BUFFER_H

#include <iostream>
#include <string>
#include <vector>
#include "glm.h"
#include "defines.h"
#include "simulator/vulkan/device.h"
#include "simulator/vulkan/buffer.h"
#include "guiStructs.h"

class GuiUniformBuffer : public Buffer
{
	public:
		GuiUniformBuffer(Device* device, VkDeviceSize size);
		~GuiUniformBuffer();

		void setValue(GuiUniformBufferObject ubo);
	private:
};

#endif// GUI_UNIFORM_BUFFER_H
