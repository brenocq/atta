//--------------------------------------------------
// Robot Simulator
// renderPass.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef RENDER_PASS_H
#define RENDER_PASS_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"
#include "swapChain.h"
#include "depthBuffer.h"
#include "colorBuffer.h"

class RenderPass
{
	public:
		RenderPass(Device* device, DepthBuffer* depthBuffer, ColorBuffer* colorBuffer);
		~RenderPass();

		VkRenderPass handle() const { return _renderPass; }
		DepthBuffer* getDepthBuffer() const { return _depthBuffer; }
		ColorBuffer* getColorBuffer() const { return _colorBuffer; }

	private:
		VkRenderPass _renderPass;
		Device* _device;
		DepthBuffer* _depthBuffer;
		ColorBuffer* _colorBuffer;
};

#endif// RENDER_PASS_H
