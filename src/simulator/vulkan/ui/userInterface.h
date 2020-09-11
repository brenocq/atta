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
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_vulkan.h"

#include "defines.h"
#include "../device.h"
#include "../window.h"
#include "../descriptorPool.h"
#include "../swapChain.h"
#include "../commandPool.h"
#include "../../scene.h"
#include "uiRenderPass.h"
#include "uiFrameBuffer.h"

// TODO use renderPass and frameBuffer from vulkan folder

class UserInterface
{
	public:
		UserInterface(Device* device, Window* window, SwapChain* swapChain, Scene* scene);
		~UserInterface();

		void draw();
		void render(int i);
		UiRenderPass* getRenderPass() const { return _imguiRenderPass; }
		VkCommandBuffer getCommandBuffer(int i) const { return _imguiCommandBuffers[i]; }

		// Topbar->View
		bool getShowPhysicsDebugger() const { return _showPhysicsDebugger; }

	private:
		void createDescriptorPool();
		void createCommandBuffers();
		static void checkResult(VkResult result);

		//--------------- IMGUI main ------------------//
		void showSceneWindow(bool* showWindow);
		void showObjectInfoWindows();
		//------------- IMGUI helpers -----------------//
		void helpMarker(std::string text);

		// Scene tree
		void createSceneTreeNode(Object* object);

		// Object menu
		void objectMenuPopup(Object* object);
		std::map<Object*, bool> _showObjectInfo;
		//---------------------------------------------//

		Device* _device;
		Window* _window;
		SwapChain* _swapChain;
		Scene* _scene;

		DescriptorPool* _imguiDescriptorPool;
		CommandPool* _imguiCommandPool;
		UiRenderPass* _imguiRenderPass;
		std::vector<UiFrameBuffer*> _imguiFrameBuffers;
		std::vector<VkCommandBuffer> _imguiCommandBuffers;

		// Topbar->View
		bool _showPhysicsDebugger;
};

#endif// USER_INTERFACE_H
