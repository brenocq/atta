//--------------------------------------------------
// Robot Simulator
// userInterface.h
// Date: 2020-07-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <iostream>
#include <string.h>

#include "defines.h"
#include "simulator/vulkan/device.h"
#include "simulator/vulkan/window.h"
#include "simulator/vulkan/swapChain.h"
#include "simulator/scene.h"
#include "simulator/vulkan/commandPool.h"
#include "simulator/vulkan/commandBuffers.h"
#include "guiPipeline.h"
#include "guiUniformBuffer.h"
#include "guiRender.h"
#include "widgets/widgets.h"
#include "font/fontLoader.h"

class UserInterface
{
	public:
		UserInterface(Device* device, Window* window, SwapChain* swapChain, Scene* scene);
		~UserInterface();

		void draw();
		void render(int i);
		VkCommandBuffer getCommandBuffer(int i) const { return _guiCommandBuffers->handle()[i]; }

		// Topbar->View
		bool getShowPhysicsDebugger() const { return _showPhysicsDebugger; }

		// Set variables
		void setEnableRayTacing(bool* enableRT) { _enableRayTracing = enableRT; }
		void setSplitRender(bool* splitRender) { _splitRender = splitRender; }

		// Window callbacks
		void onKey(int key, int scancode, int action, int mods);
		void onCursorPosition(double xpos, double ypos);
		void onMouseButton(int button, int action, int mods);
		void onScroll(double xoffset, double yoffset);

	private:
		void createWidgetTree();
		static void checkResult(VkResult result);

		std::map<Object*, bool> _showObjectInfo;

		Device* _device;
		Window* _window;
		SwapChain* _swapChain;
		Scene* _scene;

		CommandPool* _guiCommandPool;
		CommandBuffers* _guiCommandBuffers;
		std::vector<GuiUniformBuffer*> _guiUniformBuffers;
		GuiPipeline* _guiPipeline;
		guib::GuiRender* _guiRender;


		//---------- GUI variables ----------//
		guib::Widget* _rootWidget;
		std::vector<guib::Window*> _windows;

		// Topbar->Main
		bool* _enableRayTracing;
		bool* _splitRender;
		// Topbar->View
		bool _showPhysicsDebugger;

		//---------- GuiB font render ----------//
		guib::FontLoader* _fontLoader;
};

#endif// USER_INTERFACE_H
