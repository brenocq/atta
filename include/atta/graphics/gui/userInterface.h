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

#include <atta/graphics/vulkan/device.h>
#include <atta/graphics/core/window.h>
#include <atta/graphics/vulkan/swapChain.h>
#include <atta/core/scene.h>
#include <atta/graphics/vulkan/commandPool.h>
#include <atta/graphics/vulkan/commandBuffers.h>
#include <atta/graphics/vulkan/image.h>
#include <atta/graphics/vulkan/imageView.h>
#include <atta/graphics/gui/guiPipeline.h>
#include <atta/graphics/gui/guiUniformBuffer.h>
#include <atta/graphics/gui/guiRender.h>
#include <atta/graphics/gui/widgets/widgets.h>
#include <atta/graphics/gui/font/fontLoader.h>

namespace atta
{
	class UserInterface
	{
		public:
			struct CreateInfo
			{
				std::shared_ptr<vk::Device> device;
				std::shared_ptr<Window> window;
				std::shared_ptr<Scene> scene;
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
			std::shared_ptr<Scene> _scene;
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
