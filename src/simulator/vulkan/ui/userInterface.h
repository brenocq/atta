//--------------------------------------------------
// Robot Simulator
// userInterface.h
// Date: 10/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <iostream>
#include <string.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_vulkan.h"

#include "defines.h"
#include "../device.h"
#include "../window.h"
#include "../descriptorPool.h"
#include "../swapChain.h"
#include "../commandPool.h"
#include "../frameBuffer.h"
#include "uiRenderPass.h"

class UserInterface
{
	public:
	UserInterface(Device* device, Window* window, SwapChain* swapChain, CommandPool* commandPool);
	~UserInterface();

	void draw();
	void render(VkCommandBuffer commandBuffer, FrameBuffer* frameBuffer);
	UiRenderPass* getRenderPass() const { return _renderPass; }


	private:
	void createDescriptorPool();
	static void checkResult(VkResult result);

	Device* _device;
	Window* _window;
	DescriptorPool* _imguiDescriptorPool;
	SwapChain* _swapChain;
	CommandPool* _commandPool;
	UiRenderPass* _renderPass;
};

#endif// USER_INTERFACE_H
