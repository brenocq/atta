//--------------------------------------------------
// Robot Simulator
// guiRenderPass.h
// Date: 2020-07-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUI_RENDER_PASS_H
#define GUI_RENDER_PASS_H

#include <iostream>
#include <string>
#include <atta/graphics/vulkan/device.h>
#include <atta/graphics/vulkan/swapChain.h>

namespace atta
{
	class GuiRenderPass
	{
		public:
			GuiRenderPass(std::shared_ptr<vk::Device> device, VkFormat colorFormat);
			~GuiRenderPass();

			VkRenderPass handle() const { return _renderPass; }
			VkFormat getFormat() const { return _colorFormat; }

		private:
			VkRenderPass _renderPass;
			std::shared_ptr<vk::Device> _device;
			VkFormat _colorFormat;
	};
}

#endif// GUI_RENDER_PASS_H
