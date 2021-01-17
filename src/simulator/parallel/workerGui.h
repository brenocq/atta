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
#include "simulator/graphics/vulkan/vulkanCore.h"
#include "simulator/graphics/vulkan/surface.h"
#include "simulator/graphics/vulkan/swapChain.h"
#include "simulator/graphics/vulkan/commandPool.h"
#include "simulator/graphics/vulkan/commandBuffers.h"
#include "simulator/graphics/vulkan/semaphore.h"
#include "simulator/graphics/vulkan/fence.h"

namespace atta
{
	class WorkerGui : public Worker
	{
		public:
			WorkerGui(std::shared_ptr<vk::VulkanCore> vkCore);
			~WorkerGui();

			void operator()();

		private:
			void render();
			void recordCommands(VkCommandBuffer commandBuffer, unsigned imageIndex);

			std::shared_ptr<Window> _window;
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
	};
}

#endif// ATTA_WORKER_GUI_H
