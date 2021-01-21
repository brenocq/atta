//--------------------------------------------------
// Atta Parallel
// workerGui.h
// Date: 2021-01-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_WORKER_GUI_H
#define ATTA_WORKER_GUI_H

#include "worker.h"
#include "simulator/graphics/core/window.h"
#include "simulator/graphics/core/modelViewController.h"
#include "simulator/graphics/vulkan/vulkanCore.h"
#include "simulator/graphics/vulkan/surface.h"
#include "simulator/graphics/vulkan/swapChain.h"
#include "simulator/graphics/vulkan/commandPool.h"
#include "simulator/graphics/vulkan/commandBuffers.h"
#include "simulator/graphics/vulkan/semaphore.h"
#include "simulator/graphics/vulkan/fence.h"
#include "simulator/graphics/vulkan/image.h"
#include "simulator/graphics/renderers/renderer.h"
#include "simulator/graphics/gui/userInterface.h"

namespace atta
{
	class WorkerGui : public Worker
	{
		public:
			struct ImageCopy {
				std::shared_ptr<vk::Image> image;
				VkExtent2D extent;
				VkOffset2D offset;
			};

			WorkerGui(std::shared_ptr<vk::VulkanCore> vkCore);
			~WorkerGui();

			void operator()();

			//---------- Setters ----------//
			void setCommands(std::vector<std::function<void(VkCommandBuffer commandBuffer)>> commands) { _commands = commands; };
			void setImageCopies(std::vector<ImageCopy> imageCopies) { _imageCopies = imageCopies; }
			void setMainRenderer(std::shared_ptr<Renderer> mainRenderer) { _mainRenderer = mainRenderer; }

		private:
			void render();
			void recordCommands(VkCommandBuffer commandBuffer, unsigned imageIndex);
			void copyImageCommands(VkCommandBuffer commandBuffer, unsigned imageIndex, ImageCopy imageCopy);

			// Window callbacks
			void onKey(int key, int scancode, int action, int mods);
			void onCursorPosition(double xpos, double ypos);
			void onMouseButton(int button, int action, int mods);
			void onScroll(double xoffset, double yoffset);

			std::shared_ptr<Window> _window;
			std::shared_ptr<ModelViewController> _modelViewController;
			std::shared_ptr<Renderer> _mainRenderer;
			std::shared_ptr<vk::VulkanCore> _vkCore;

			std::shared_ptr<vk::Surface> _surface;
			std::shared_ptr<vk::SwapChain> _swapChain;
			std::shared_ptr<vk::CommandPool> _commandPool;
			std::shared_ptr<vk::CommandBuffers> _commandBuffers;

			// Presentation sync objects
			std::vector<std::shared_ptr<vk::Semaphore>> _imageAvailableSemaphores;
			std::vector<std::shared_ptr<vk::Semaphore>> _renderFinishedSemaphores;
			std::vector<std::shared_ptr<vk::Fence>> _inFlightFences;
			std::vector<VkFence> _imagesInFlight;
			int _currentFrame;

			// Work to process
			std::vector<std::function<void(VkCommandBuffer commandBuffer)>> _commands;

			// Images to copy
			std::vector<ImageCopy> _imageCopies;

			// User interface
			std::shared_ptr<UserInterface> _ui;
	};
}

#endif// ATTA_WORKER_GUI_H
