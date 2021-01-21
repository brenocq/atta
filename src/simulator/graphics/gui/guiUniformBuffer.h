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
#include "defines.h"
#include "simulator/graphics/vulkan/device.h"
#include "simulator/graphics/vulkan/buffer.h"
#include "guiStructs.h"

namespace atta
{
	class GuiUniformBuffer : public vk::Buffer
	{
		public:
			GuiUniformBuffer(std::shared_ptr<vk::Device> device, VkDeviceSize size);
			~GuiUniformBuffer();

			void setValue(GuiUniformBufferObject ubo);
		private:
	};
}

#endif// GUI_UNIFORM_BUFFER_H
