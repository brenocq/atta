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
#include "uiRenderPass.h"
#include "uiFrameBuffer.h"

class UserInterface
{
	public:
	UserInterface(Device* device, Window* window, SwapChain* swapChain);
	~UserInterface();

	void draw();
	void render(int i);
	UiRenderPass* getRenderPass() const { return _imguiRenderPass; }
	VkCommandBuffer getCommandBuffer(int i) const { return _imguiCommandBuffers[i]; }

	private:
	void createDescriptorPool();
	void createCommandBuffers();
	static void checkResult(VkResult result);

	Device* _device;
	Window* _window;
	SwapChain* _swapChain;

	DescriptorPool* _imguiDescriptorPool;
	CommandPool* _imguiCommandPool;
	UiRenderPass* _imguiRenderPass;
	std::vector<UiFrameBuffer*> _imguiFrameBuffers;
	std::vector<VkCommandBuffer> _imguiCommandBuffers;
};

#endif// USER_INTERFACE_H
