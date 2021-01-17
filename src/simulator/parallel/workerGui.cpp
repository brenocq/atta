//--------------------------------------------------
// Atta Parallel
// workerGui.cpp
// Date: 2021-01-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "workerGui.h"
#include <iostream>
#include "simulator/helpers/log.h"
#include "simulator/graphics/vulkan/imageMemoryBarrier.h"

namespace atta
{
	WorkerGui::WorkerGui(std::shared_ptr<vk::VulkanCore> vkCore):
		_vkCore(vkCore)
	{
		// Create window (GUI thread only)
		_window = std::make_shared<Window>();

		// Vulkan objects
		_surface = std::make_shared<vk::Surface>(_vkCore->getInstance(), _window);
		_swapChain = std::make_shared<vk::SwapChain>(_vkCore->getDevice(), _surface);
		_commandPool = std::make_shared<vk::CommandPool>(_vkCore->getDevice(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
		_commandBuffers = std::make_shared<vk::CommandBuffers>(_vkCore->getDevice(), _commandPool, _swapChain->getImages().size());

		// Create render sync objects
		_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        _renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        _inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        _imagesInFlight.resize(_swapChain->getImages().size(), VK_NULL_HANDLE);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			_imageAvailableSemaphores[i] = std::make_shared<vk::Semaphore>(_vkCore->getDevice());
			_renderFinishedSemaphores[i] = std::make_shared<vk::Semaphore>(_vkCore->getDevice());
			_inFlightFences[i] = std::make_shared<vk::Fence>(_vkCore->getDevice());
		}
	}

	WorkerGui::~WorkerGui()
	{
		std::cout << "GUI worker destroyed\n";
	}

	void WorkerGui::operator()()
	{
		// TODO check _window->shouldClose();
		while(true)
		{
			render();
			_window->poolEvents();
		}
	}

	void WorkerGui::render()
	{
		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(_vkCore->getDevice()->handle(), _swapChain->handle(), UINT64_MAX, _imageAvailableSemaphores[_currentFrame]->handle(), VK_NULL_HANDLE, &imageIndex);

		//---------- Check swapchain ----------//
		if(result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			// Recreate the swapchain (window resized)
			Log::warning("WorkerGui", "Swap chain out of date! Must reset the swapChain!");
			return;
		}
		else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			Log::error("WorkerGui", "Failed to acquire swap chain image!");
			exit(1);
		}

		//---------- Record to command buffer ----------//
		VkCommandBuffer commandBuffer = _commandBuffers->begin(imageIndex);
		{
			recordCommands(commandBuffer, imageIndex);
		}
		_commandBuffers->end(imageIndex);

		//---------- CPU-GPU syncronization ----------//
		_inFlightFences[_currentFrame]->wait(UINT64_MAX);
		_inFlightFences[_currentFrame]->reset();

		//---------- GPU-GPU syncronization ----------//
		VkSemaphore waitSemaphores[] = {_imageAvailableSemaphores[_currentFrame]->handle()};
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		VkSemaphore signalSemaphores[] = {_renderFinishedSemaphores[_currentFrame]->handle()};

		//---------- Submit to graphics queue ----------//
		std::array<VkCommandBuffer, 1> submitCommandBuffers = { _commandBuffers->handle()[imageIndex] };

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;// Wait image available
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = static_cast<uint32_t>(submitCommandBuffers.size());
		submitInfo.pCommandBuffers = submitCommandBuffers.data();
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;// Signal image rendered

		_inFlightFences[_currentFrame]->reset();

		if(vkQueueSubmit(_vkCore->getDevice()->getGraphicsQueue(), 1, &submitInfo, _inFlightFences[_currentFrame]->handle()) != VK_SUCCESS)
		{
			Log::error("WorkerGui", "Failed to submit draw command buffer!");
			exit(1);
		}

		//---------- Submit do present queue ----------//
		VkSwapchainKHR swapChains[] = {_swapChain->handle()};
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;// Wait image rendered
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		result = vkQueuePresentKHR(_vkCore->getDevice()->getPresentQueue(), &presentInfo);
		if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			// TODO update frameBufferResized variable
			Log::warning("WorkerGui", "Framebuffer resized! Must reset the swapChain!");
		} 
		else if(result != VK_SUCCESS) 
		{
			Log::error("WorkerGui", "Failed to present swap chain image!");
			exit(1);
		}

		//---------- Next frame ----------//
		_currentFrame = (_currentFrame + 1) % _inFlightFences.size();
	}

	void WorkerGui::recordCommands(VkCommandBuffer commandBuffer, unsigned imageIndex)
	{
		VkImageSubresourceRange subresourceRange;
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.baseArrayLayer = 0;
		subresourceRange.layerCount = 1;

		vk::ImageMemoryBarrier::insert(commandBuffer, _swapChain->getImages()[imageIndex], subresourceRange, VK_ACCESS_TRANSFER_WRITE_BIT,
			//0, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
			0, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
	}
}
