//--------------------------------------------------
// Robot Simulator
// guiRenderPass.h
// Date: 2020-07-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUI_RENDER_PASS_H
#define GUI_RENDER_PASS_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "../device.h"
#include "../swapChain.h"

class GuiRenderPass
{
	public:
		GuiRenderPass(Device* device, VkFormat colorFormat);
		~GuiRenderPass();

		VkRenderPass handle() const { return _renderPass; }
		VkFormat getFormat() const { return _colorFormat; }

	private:
		VkRenderPass _renderPass;
		Device* _device;
		VkFormat _colorFormat;
};

#endif// GUI_RENDER_PASS_H
