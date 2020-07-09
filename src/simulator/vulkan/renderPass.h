//--------------------------------------------------
// Robot Simulator
// renderPass.h
// Date: 24/06/2020
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
	RenderPass(Device* device, SwapChain* swapChain, DepthBuffer* depthBuffer, ColorBuffer* colorBuffer);
	~RenderPass();

	VkRenderPass handle() const { return _renderPass; }
	SwapChain* getSwapChain() const { return _swapChain; }
	DepthBuffer* getDepthBuffer() const { return _depthBuffer; }
	ColorBuffer* getColorBuffer() const { return _colorBuffer; }

	private:
    VkRenderPass _renderPass;
	Device* _device;
	SwapChain* _swapChain;
	DepthBuffer* _depthBuffer;
	ColorBuffer* _colorBuffer;
};

#endif// RENDER_PASS_H
