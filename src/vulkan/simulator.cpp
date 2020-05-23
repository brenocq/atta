#include "simulator.h"

Simulator::Simulator()
{
	_isWireFrame = false;

	initWindow();
	initInstance();
	initSurface();
	initDevice();
	initSwapChain();
}

Simulator::~Simulator()
{
}

void Simulator::initWindow()
{
	WindowConfig windowConfig = {};
	windowConfig.title = "Breno Queiroz Simulator";
	windowConfig.width = 1366;
	windowConfig.height = 768;
	windowConfig.cursorDisabled = false;
	windowConfig.fullscreen = false;
	windowConfig.resizable = false;

	_window = new Window(windowConfig);
}

void Simulator::initInstance()
{
	// Validation layers will only be used if ENABLE_VALIDATION_LAYERS is set to true
	const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
	_instance = new Instance(_window, validationLayers);
}

void Simulator::initSurface()
{
	_surface = new Surface(_instance);
}

void Simulator::initDevice()
{
	_physicalDevice = new PhysicalDevice(_instance);
	_device = new Device(_physicalDevice->handle(), _surface);
	_commandPool = new CommandPool(_device, _device->graphicsFamilyIndex(), false);
}

void Simulator::initSwapChain()
{
	_swapChain = new SwapChain(_device);
	_depthBuffer = new DepthBuffer(_commandPool, _swapChain->extent());

	for (size_t i = 0; i != _swapChain->imageViews().size(); i++)
	{
		_imageAvailableSemaphores.emplace_back(_device);
		_renderFinishedSemaphores.emplace_back(_device);
		_inFlightFences.emplace_back(_device, true);
		//uniformBuffers_.emplace_back(*device_);
	}

	_graphicsPipeline = new GraphicsPipeline(_swapChain, _depthBuffer/*, _uniformBuffers, getScene()*/, false);

	for (const auto imageView : _swapChain->imageViews())
	{
		_swapChainFramebuffers.emplace_back(imageView, _graphicsPipeline->renderPass());
	}
	
	_commandBuffers = new CommandBuffers(_commandPool, static_cast<uint32_t>(_swapChainFramebuffers.size()));
}

void Simulator::run()
{
	_currentFrame = 0;


	_window->drawFrame = [this]() { drawFrame(); };
	_window->run();
}

void Simulator::drawFrame()
{
	const auto noTimeout = std::numeric_limits<uint64_t>::max();

	auto& inFlightFence = _inFlightFences[_currentFrame];
	const auto imageAvailableSemaphore = _imageAvailableSemaphores[_currentFrame].handle();
	const auto renderFinishedSemaphore = _renderFinishedSemaphores[_currentFrame].handle();

	inFlightFence.wait(noTimeout);

	// GET NEXT IMAGE
	uint32_t imageIndex;
	// Get index of next image to be drawn to, and signal semaphore when ready to be drawn to
	auto result = vkAcquireNextImageKHR(_device->handle(), _swapChain->handle(), noTimeout, imageAvailableSemaphore, nullptr, &imageIndex);

	//if (result == VK_ERROR_OUT_OF_DATE_KHR || _isWireFrame!= _graphicsPipeline->isWireFrame())
	//{
	//	//RecreateSwapChain();
	//	return;
	//}

	//if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	//{
	//	//Throw(std::runtime_error(std::string("failed to acquire next image (") + ToString(result) + ")"));
	//	std::cout << BOLDRED << "[Simulator] Failed to acquire next image!" << RESET << std::endl;
	//	exit(1);
	//}


	//UpdateUniformBuffer(imageIndex);
	const auto commandBuffer = _commandBuffers->begin(imageIndex);
	render(commandBuffer, imageIndex);
	_commandBuffers->end(imageIndex);


	// SUBMIT COMMAND BUFFER TO RENDER
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkCommandBuffer commandBuffers[]{ commandBuffer };
	VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };

	// Wait availableImage semaphore to submit, signal renderFinished when finished 
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = commandBuffers;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	inFlightFence.reset();

	if(vkQueueSubmit(_device->graphicsQueue(), 1, &submitInfo, inFlightFence.handle()) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[Simulator] Failed to submit draw command buffer!" << RESET << std::endl;
		exit(1);
	}

	// PRESENT RENDERED IMAGE TO SCREEN
	VkSwapchainKHR swapChains[] = { _swapChain->handle() };
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	result = vkQueuePresentKHR(_device->presentQueue(), &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		//RecreateSwapChain();
		return;
	}
	
	if (result != VK_SUCCESS)
	{
		//Throw(std::runtime_error(std::string("failed to present next image (") + ToString(result) + ")"));
		std::cout << BOLDRED << "[Simulator] Failed to present next image!" << RESET << std::endl;
	}

	_currentFrame = (_currentFrame + 1) % _inFlightFences.size();
}

void Simulator::render(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
	std::array<VkClearValue, 1> clearValues = {};
	clearValues[0].color = { {1.0f, 0.6f, 0.4f, 1.0f} };
	//clearValues[1].depthStencil = { 1.0f, 0 };

	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = _graphicsPipeline->renderPass()->handle();
	renderPassInfo.framebuffer = _swapChainFramebuffers[imageIndex].handle();
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = _swapChain->extent();
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	{
		// Bind Pipeline to be used in render pass
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline->handle());
		// Execute pipeline
		vkCmdDraw(commandBuffer, 3, 1, 0, 0);
	}
	vkCmdEndRenderPass(commandBuffer);
}
