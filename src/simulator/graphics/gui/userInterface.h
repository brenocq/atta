//--------------------------------------------------
// Robot Simulator
// userInterface.h
// Date: 2020-07-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <iostream>
#include <string>

#include "defines.h"
#include "simulator/graphics/vulkan/device.h"
#include "simulator/graphics/core/window.h"
#include "simulator/graphics/vulkan/swapChain.h"
#include "simulator/core/scene.h"
#include "simulator/graphics/vulkan/commandPool.h"
#include "simulator/graphics/vulkan/commandBuffers.h"
#include "simulator/graphics/vulkan/image.h"
#include "simulator/graphics/vulkan/imageView.h"
#include "guiPipeline.h"
#include "guiUniformBuffer.h"
#include "guiRender.h"
#include "widgets/widgets.h"
#include "font/fontLoader.h"

namespace atta
{
	class UserInterface
	{
		public:
			struct CreateInfo
			{
				std::shared_ptr<vk::Device> device;
				std::shared_ptr<Window> window;
				std::shared_ptr<vk::SwapChain> swapChain;
			};

			UserInterface(CreateInfo info);
			~UserInterface();

			void render(int imageIndex);
			void render(VkCommandBuffer commandBuffer, int imageIndex);

			// Window callbacks
			void onKey(int key, int scancode, int action, int mods);
			void onCursorPosition(double xpos, double ypos);
			void onMouseButton(int button, int action, int mods);
			void onScroll(double xoffset, double yoffset);

		private:
			void createWidgetTree();

			std::shared_ptr<vk::Device> _device;
			std::shared_ptr<Window> _window;
			std::shared_ptr<vk::SwapChain> _swapChain;

			//---------- Rendering ----------//
			std::shared_ptr<vk::CommandPool> _guiCommandPool;
			std::shared_ptr<vk::CommandBuffers> _guiCommandBuffers;
			std::shared_ptr<GuiUniformBuffer> _guiUniformBuffer;
			std::shared_ptr<GuiPipeline> _guiPipeline;
			std::shared_ptr<guib::GuiRender> _guiRender;

			//---------- GuiB root ----------//
			guib::Widget* _rootWidget;
			std::vector<guib::Window*> _windows;

			//---------- GuiB font render ----------//
			std::shared_ptr<guib::FontLoader> _fontLoader;
	};
}

#endif// USER_INTERFACE_H
