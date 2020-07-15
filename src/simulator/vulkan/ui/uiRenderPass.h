//--------------------------------------------------
// Robot Simulator
// uiRenderPass.h
// Date: 12/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef UI_RENDER_PASS_H
#define UI_RENDER_PASS_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "../device.h"
#include "../swapChain.h"

class UiRenderPass
{
	public:
	UiRenderPass(Device* device, SwapChain* swapChain);
	~UiRenderPass();

	VkRenderPass handle() const { return _renderPass; }
	SwapChain* getSwapChain() const { return _swapChain; }

	private:
    VkRenderPass _renderPass;
	Device* _device;
	SwapChain* _swapChain;
};

#endif// UI_RENDER_PASS_H
