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

class RenderPass
{
	public:
	RenderPass(Device* device, SwapChain* swapChain);
	~RenderPass();

	VkRenderPass handle() const { return _renderPass; }
	SwapChain* getSwapChain() const { return _swapChain; }

	private:
    VkRenderPass _renderPass;
	Device* _device;
	SwapChain* _swapChain;
};

#endif// RENDER_PASS_H
