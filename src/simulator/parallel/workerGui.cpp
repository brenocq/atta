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
		_vkCore(vkCore), _currentFrame(0)
	{
		// Create window (GUI thread only)
		_window = std::make_shared<Window>();
		_window->windowResized = [this](){ };
		_window->onKey = [this](const int key, const int scancode, const int action, const int mods) { onKey(key, scancode, action, mods); };
		_window->onCursorPosition = [this](const double xpos, const double ypos) { onCursorPosition(xpos, ypos); };
		_window->onMouseButton = [this](const int button, const int action, const int mods) { onMouseButton(button, action, mods); };
		_window->onScroll = [this](const double xoffset, const double yoffset) { onScroll(xoffset, yoffset); };

		// Create model view controller
		_modelViewController = std::make_shared<ModelViewController>();

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

		// Create user interface objects
		UserInterface::CreateInfo uiInfo =
		{
			.device = _vkCore->getDevice(),
			.window = _window,
			.swapChain = _swapChain
		};
		_ui = std::make_shared<UserInterface>(uiInfo);
	}

	WorkerGui::~WorkerGui()
	{
		std::cout << "GUI worker destroyed\n";
	}

	void WorkerGui::operator()()
	{
		while(!_window->shouldClose())
		{
			_modelViewController->updateCamera(0.01);
			render();
			_window->poolEvents();
		}
		// Send signal to close atta simulator
	}

	void WorkerGui::setMainRenderer(std::shared_ptr<Renderer> mainRenderer)
	{
		_mainRenderer = mainRenderer;
		_modelViewController->reset(mainRenderer->getViewMatrix());
	}

	void WorkerGui::render()
	{
		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(_vkCore->getDevice()->handle(), _swapChain->handle(), 
				UINT64_MAX, _imageAvailableSemaphores[_currentFrame]->handle(), VK_NULL_HANDLE, &imageIndex);

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

		// Update main renderer camera`
		// TODO think
		_mainRenderer->updateCameraMatrix(_modelViewController->getModelView());

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
		_currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void WorkerGui::recordCommands(VkCommandBuffer commandBuffer, unsigned imageIndex)
	{
		VkImageSubresourceRange subresourceRange;
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.baseArrayLayer = 0;
		subresourceRange.layerCount = 1;

		//---------- Run commands (renderers) ----------//
		for(auto command : _commands)
			command(commandBuffer);

		//---------- Copy main renderer image ----------//
		vk::ImageMemoryBarrier::insert(commandBuffer, _swapChain->getImages()[imageIndex], subresourceRange, VK_ACCESS_TRANSFER_WRITE_BIT,
			0, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		for(auto imageCopy : _imageCopies)
			copyImageCommands(commandBuffer, imageIndex, imageCopy);

		vk::ImageMemoryBarrier::insert(commandBuffer, _swapChain->getImages()[imageIndex], subresourceRange, VK_ACCESS_TRANSFER_WRITE_BIT,
			0, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

		//---------- Render user interface on top of it----------//
		_ui->render(commandBuffer, imageIndex);

		//---------- Change layout to present image ----------//
		vk::ImageMemoryBarrier::insert(commandBuffer, _swapChain->getImages()[imageIndex], subresourceRange, VK_ACCESS_TRANSFER_WRITE_BIT,
			0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
	}

	void WorkerGui::copyImageCommands(VkCommandBuffer commandBuffer, unsigned imageIndex, ImageCopy imageCopy)
	{
		// Change src image layout to transfer src
		VkImageSubresourceRange subresourceRange;
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.baseArrayLayer = 0;
		subresourceRange.layerCount = 1;

		vk::ImageMemoryBarrier::insert(commandBuffer, imageCopy.image->handle(), subresourceRange, VK_ACCESS_TRANSFER_WRITE_BIT,
			0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

		// Copy src image to dst image
		VkExtent2D srcExtent = imageCopy.image->getExtent();
		VkExtent2D dstExtent = _swapChain->getImageExtent();
		VkImageBlit blit{};
		blit.srcOffsets[0] = {0, 0, 0};
		blit.srcOffsets[1] = { srcExtent.width, srcExtent.height, 1 };
		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel = 0;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = {0, 0, 0};
		blit.dstOffsets[1] = { dstExtent.width, dstExtent.height, 1 };
		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel = 0;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		vkCmdBlitImage(commandBuffer,
			imageCopy.image->handle(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			_swapChain->getImages()[imageIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &blit,
			VK_FILTER_LINEAR);
	}

	//------------------------------------------------------------------------------//
	//------------------------------ Window Callbacks ------------------------------//
	//------------------------------------------------------------------------------//
	void WorkerGui::onKey(int key, int scancode, int action, int mods)
	{
		switch(key)
		{
			case GLFW_KEY_ESCAPE:
				_window->close();
				break;
		}

		_modelViewController->onKey(key, scancode, action, mods);
		_ui->onKey(key, scancode, action, mods);
	}

	void WorkerGui::onCursorPosition(double xpos, double ypos)
	{
		_modelViewController->onCursorPosition(xpos, ypos);
		_ui->onCursorPosition(xpos, ypos);
	}

	void WorkerGui::onMouseButton(int button, int action, int mods)
	{
		switch(button)
		{
			case GLFW_MOUSE_BUTTON_MIDDLE:
				_window->toggleCursorVisibility();
				break;
		}

		_modelViewController->onMouseButton(button, action, mods);
		_ui->onMouseButton(button, action, mods);
	}

	void WorkerGui::onScroll(double xoffset, double yoffset)
	{
		_modelViewController->onScroll(xoffset, yoffset);
		_ui->onScroll(xoffset, yoffset);
	}
}
