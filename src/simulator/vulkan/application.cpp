//--------------------------------------------------
// Robot Simulator
// application.cpp
// Date: 21/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "application.h"

Application::Application()
{
	_window = new Window();
	_instance = new Instance();
	_debugMessenger = new DebugMessenger(_instance);
	_surface = new Surface(_instance, _window);
	_physicalDevice = new PhysicalDevice(_instance, _surface);
	_device = new Device(_physicalDevice);
	_swapChain = new SwapChain(_device, _window);
	_graphicsPipeline = new GraphicsPipeline(_device, _swapChain);

	_frameBuffers.resize(_swapChain->getImageViews().size());
	for(size_t i = 0; i < _frameBuffers.size(); i++) 
	{
		_frameBuffers[i] = new FrameBuffer(_swapChain->getImageViews()[i], _graphicsPipeline->getRenderPass());
	}

	_commandPool = new CommandPool(_device);
	//_commandBuffers = new CommandBuffers(_device, _commandPool, _frameBuffers);
	//render();
	createCommandBuffers();

	_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	_imagesInFlight.resize(_swapChain->getImages().size(), VK_NULL_HANDLE);
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
	{
		_imageAvailableSemaphores[i] = new Semaphore(_device);
		_renderFinishedSemaphores[i] = new Semaphore(_device);
		_inFlightFences[i] = new Fence(_device);
	}
}

Application::~Application()
{
	for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
	{
		delete _imageAvailableSemaphores[i];
		_imageAvailableSemaphores[i] = nullptr;
		delete _renderFinishedSemaphores[i];
		_renderFinishedSemaphores[i] = nullptr;
		delete _inFlightFences[i];
		_inFlightFences[i] = nullptr;
    }

	//for(size_t i = 0; i < (size_t)_imagesInFlight.size(); i++){
	//	delete _imagesInFlight[i];
	//	_imagesInFlight[i] = nullptr;
	//}

	delete _commandPool;
	_commandPool = nullptr;

	for (auto frameBuffer : _frameBuffers) 
	{
		delete frameBuffer;
		frameBuffer = nullptr;
    }

	delete _graphicsPipeline;
	_graphicsPipeline = nullptr;

	delete _swapChain;
	_swapChain = nullptr;

	delete _device;
	_device = nullptr;

	delete _debugMessenger;
	_debugMessenger = nullptr;

	delete _surface;
	_surface = nullptr;

	delete _instance;
	_instance = nullptr;

	delete _window;
	_window = nullptr;

	delete _physicalDevice;
	_physicalDevice = nullptr;
}

void Application::run()
{
	//for(int i=0; i<_frameBuffers.size(); i++)
	//	render(i);

	_window->drawFrame = [this]() { drawFrame(); };
	_window->loop();
	vkDeviceWaitIdle(_device->handle());
}

void Application::drawFrame()
{
	_inFlightFences[_currentFrame]->wait(UINT64_MAX);
	_inFlightFences[_currentFrame]->reset();
	//vkWaitForFences(_device->handle(), 1, &_inFlightFences[_currentFrame]->handle(), VK_TRUE, UINT64_MAX);
    //vkResetFences(_device->handle(), 1, &_inFlightFences[_currentFrame]->handle());

	uint32_t imageIndex;
    vkAcquireNextImageKHR(_device->handle(), _swapChain->handle(), UINT64_MAX, _imageAvailableSemaphores[_currentFrame]->handle(), VK_NULL_HANDLE, &imageIndex);
	
	// Check if a previous frame is using this image (i.e. there is its fence to wait on)
    if (_imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(_device->handle(), 1, &_imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    // Mark the image as now being in use by this frame
    _imagesInFlight[imageIndex] = _inFlightFences[_currentFrame]->handle();

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {_imageAvailableSemaphores[_currentFrame]->handle()};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &_commandBuffersTest[imageIndex];

	VkSemaphore signalSemaphores[] = {_renderFinishedSemaphores[_currentFrame]->handle()};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	_inFlightFences[_currentFrame]->reset();

	if (vkQueueSubmit(_device->getGraphicsQueue(), 1, &submitInfo, _inFlightFences[_currentFrame]->handle()) != VK_SUCCESS) {
		std::cout << BOLDRED << "[Application]" << RESET << RED << " Failed to submit draw command buffer!" << RESET << std::endl;
		exit(1);
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = {_swapChain->handle()};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(_device->getPresentQueue(), &presentInfo);
	vkQueueWaitIdle(_device->getPresentQueue());

	_currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Application::render(int i)
{
	std::vector<VkCommandBuffer> commandBuffers = _commandBuffers->handle();
	//for(size_t i = 0; i < commandBuffers.size(); i++) 
	//{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		const auto commandBuffer = _commandBuffers->begin(i);
		{
			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = _graphicsPipeline->getRenderPass()->handle();
			renderPassInfo.framebuffer = _frameBuffers[i]->handle();
			renderPassInfo.renderArea.offset = {0, 0};
			renderPassInfo.renderArea.extent = _swapChain->getExtent();

			VkClearValue clearColor = {1.0f, 0.0f, 0.0f, 1.0f};
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			{
				vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline->handle());

				vkCmdDraw(commandBuffer, 3, 1, 0, 0);
			}
			vkCmdEndRenderPass(commandBuffer);

		}
		_commandBuffers->end(i);
	//}
	_commandBuffers->setHandle(commandBuffers);
}

void Application::createCommandBuffers()
{
	// TODO put it in a beautiful way
	_commandBuffersTest.resize(_frameBuffers.size());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = _commandPool->handle();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t) _commandBuffersTest.size();

	if (vkAllocateCommandBuffers(_device->handle(), &allocInfo, _commandBuffersTest.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}

	for (size_t i = 0; i < _commandBuffersTest.size(); i++) {
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(_commandBuffersTest[i], &beginInfo) != VK_SUCCESS) {
			std::cout << BOLDRED << "[CommandBuffers]" << RESET << RED << " Failed to begin recording command buffer!" << RESET << std::endl;
			exit(1);
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = _graphicsPipeline->getRenderPass()->handle();
		renderPassInfo.framebuffer = _frameBuffers[i]->handle();
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = _swapChain->getExtent();

		VkClearValue clearColor = {1.0f, 1.0f, 1.0f, 1.0f};
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(_commandBuffersTest[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(_commandBuffersTest[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline->handle());

			vkCmdDraw(_commandBuffersTest[i], 9, 1, 0, 0);

		vkCmdEndRenderPass(_commandBuffersTest[i]);

		if (vkEndCommandBuffer(_commandBuffersTest[i]) != VK_SUCCESS) {
			std::cout << BOLDRED << "[CommandBuffers]" << RESET << RED << " Failed to record command buffer!" << RESET << std::endl;
			exit(1);
		}
	}
}
