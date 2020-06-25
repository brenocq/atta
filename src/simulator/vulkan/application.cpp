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
	_commandBuffers = new CommandBuffers(_device, _commandPool, _frameBuffers);
	//render();

	_imageAvailableSemaphore = new Semaphore(_device);
	_renderFinishedSemaphore = new Semaphore(_device);
}

Application::~Application()
{
	delete _imageAvailableSemaphore;
	_imageAvailableSemaphore = nullptr;

	delete _renderFinishedSemaphore;
	_renderFinishedSemaphore = nullptr;

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
	_window->drawFrame = [this]() { drawFrame(); };
	_window->loop();
}

void Application::drawFrame()
{
	uint32_t imageIndex;
    vkAcquireNextImageKHR(_device->handle(), _swapChain->handle(), UINT64_MAX, _imageAvailableSemaphore->handle(), VK_NULL_HANDLE, &imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {_imageAvailableSemaphore->handle()};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	std::cout << "image: " << imageIndex << " cb:" << _commandBuffers->handle().size() << "\n";
	submitInfo.pCommandBuffers = &_commandBuffers->handle()[imageIndex];

	VkSemaphore signalSemaphores[] = {_renderFinishedSemaphore->handle()};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(_device->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
		std::cout << BOLDRED << "[Application]" << RESET << RED << " Failed to submit draw command buffer!" << RESET << std::endl;
		exit(1);
	}

	render(imageIndex);

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = {_swapChain->handle()};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(_device->getPresentQueue(), &presentInfo);

	//currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
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
