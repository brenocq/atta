//--------------------------------------------------
// Robot Simulator
// application.cpp
// Date: 21/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "application.h"

const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4
};

Application::Application():
	_currentFrame(0), _framebufferResized(false)
{
	_window = new Window();
	_instance = new Instance();
	_debugMessenger = new DebugMessenger(_instance);
	_surface = new Surface(_instance, _window);
	_physicalDevice = new PhysicalDevice(_instance, _surface);
	_device = new Device(_physicalDevice);
	_commandPool = new CommandPool(_device);
	_swapChain = new SwapChain(_device, _window);
	_descriptorSetLayout = new DescriptorSetLayout(_device);
	_depthBuffer = new DepthBuffer(_device, _commandPool, _swapChain->getExtent());
	_graphicsPipeline = new GraphicsPipeline(_device, _swapChain, _depthBuffer, _descriptorSetLayout);
	_frameBuffers.resize(_swapChain->getImageViews().size());
	for(size_t i = 0; i < _frameBuffers.size(); i++) 
	{
		_frameBuffers[i] = new FrameBuffer(_swapChain->getImageViews()[i], _graphicsPipeline->getRenderPass());
	}

	_model = new Model(_device, _commandPool, "viking_room");
	createBuffers();

	_descriptorPool = new DescriptorPool(_device, _swapChain->getImages().size());
	//_texture = new Texture(_device, _commandPool, "assets/textures/texture.jpg");
	_descriptorSets = new DescriptorSets(_device, _descriptorPool, _descriptorSetLayout, _uniformBuffers, _model->getTexture());

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
	cleanupSwapChain();

	delete _depthBuffer;
	_depthBuffer = nullptr;

	delete _model;
	_model = nullptr;

	delete _descriptorSetLayout;
	_descriptorSetLayout = nullptr;

	delete _indexBuffer;
	_indexBuffer = nullptr;
	delete _vertexBuffer;
	_vertexBuffer = nullptr;

	for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
	{
		delete _renderFinishedSemaphores[i];
		_renderFinishedSemaphores[i] = nullptr;
		delete _imageAvailableSemaphores[i];
		_imageAvailableSemaphores[i] = nullptr;
		delete _inFlightFences[i];
		_inFlightFences[i] = nullptr;
    }

	delete _commandPool;
	_commandPool = nullptr;

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

void Application::cleanupSwapChain()
{
	delete _depthBuffer;
	_depthBuffer = nullptr;

	for (auto frameBuffer : _frameBuffers) 
	{
		delete frameBuffer;
		frameBuffer = nullptr;
    }

	vkFreeCommandBuffers(_device->handle(), _commandPool->handle(), static_cast<uint32_t>(_commandBuffersTest.size()), _commandBuffersTest.data());

	delete _graphicsPipeline;
	_graphicsPipeline = nullptr;

	delete _swapChain;
	_swapChain = nullptr;

	for (auto uniformBuffer : _uniformBuffers) 
	{
		delete uniformBuffer;
		uniformBuffer = nullptr;
    }

	delete _descriptorPool;
	_descriptorPool = nullptr;
}

void Application::recreateSwapChain()
{
	_window->waitIfMinimized();
	vkDeviceWaitIdle(_device->handle());

	cleanupSwapChain();

	_swapChain = new SwapChain(_device, _window);
	_depthBuffer = new DepthBuffer(_device, _commandPool, _swapChain->getExtent());
	_graphicsPipeline = new GraphicsPipeline(_device, _swapChain, _depthBuffer, _descriptorSetLayout);
	_frameBuffers.resize(_swapChain->getImageViews().size());

	for(size_t i = 0; i < _frameBuffers.size(); i++) 
	{
		_frameBuffers[i] = new FrameBuffer(_swapChain->getImageViews()[i], _graphicsPipeline->getRenderPass());
	}

	_uniformBuffers.resize(_swapChain->getImages().size());
	for(size_t i = 0; i < _swapChain->getImages().size(); i++) 
	{
		_uniformBuffers[i] = new UniformBuffer(_device, (int)sizeof(UniformBufferObject));
    }

	_descriptorPool = new DescriptorPool(_device, _swapChain->getImages().size());
	createCommandBuffers();
}

void Application::run()
{
	//for(int i=0; i<_frameBuffers.size(); i++)
	//	render(i);

	_window->drawFrame = [this]() { drawFrame(); };
	_window->windowResized = [this]() { framebufferResizeCallback(); };
	_window->loop();
	vkDeviceWaitIdle(_device->handle());
}

void Application::drawFrame()
{
	_inFlightFences[_currentFrame]->wait(UINT64_MAX);
	_inFlightFences[_currentFrame]->reset();

	uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(_device->handle(), _swapChain->handle(), UINT64_MAX, _imageAvailableSemaphores[_currentFrame]->handle(), VK_NULL_HANDLE, &imageIndex);

	// Check if is necessary to recreate the swapchain (window resized)
	if(result == VK_ERROR_OUT_OF_DATE_KHR) 
	{
		recreateSwapChain();
		return;
	} 
	else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) 
	{
		std::cout << BOLDRED << "[Application]" << RESET << RED << " Failed to acquire swap chain image!" << RESET << std::endl;
		exit(1);
	}
	
	// Check if a previous frame is using this image (i.e. there is its fence to wait on)
    if (_imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(_device->handle(), 1, &_imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    // Mark the image as now being in use by this frame
    _imagesInFlight[imageIndex] = _inFlightFences[_currentFrame]->handle();

	updateUniformBuffer(imageIndex);

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

	result = vkQueuePresentKHR(_device->getPresentQueue(), &presentInfo);
	if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _framebufferResized)
	{
		_framebufferResized = false;
		recreateSwapChain();
	} 
	else if(result != VK_SUCCESS) 
	{
		std::cout << BOLDRED << "[Application]" << RESET << RED << " Failed to present swap chain image!" << RESET << std::endl;
		exit(1);
	}

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

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
		clearValues[1].depthStencil = {1.0f, 0};
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(_commandBuffersTest[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(_commandBuffersTest[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline->handle());

			VkBuffer vertexBuffers[] = {_vertexBuffer->handle()};
			VkDeviceSize offsets[] = {0};
			vkCmdBindVertexBuffers(_commandBuffersTest[i], 0, 1, vertexBuffers, offsets);
			vkCmdBindIndexBuffer(_commandBuffersTest[i], _indexBuffer->handle(), 0, VK_INDEX_TYPE_UINT32);

			vkCmdBindDescriptorSets(_commandBuffersTest[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline->getPipelineLayout()->handle(), 0, 1, &_descriptorSets->handle()[i], 0, nullptr);
			vkCmdDrawIndexed(_commandBuffersTest[i], static_cast<uint32_t>(_model->getIndices().size()), 1, 0, 0, 0);

		vkCmdEndRenderPass(_commandBuffersTest[i]);

		if (vkEndCommandBuffer(_commandBuffersTest[i]) != VK_SUCCESS) {
			std::cout << BOLDRED << "[CommandBuffers]" << RESET << RED << " Failed to record command buffer!" << RESET << std::endl;
			exit(1);
		}
	}
}

void Application::createBuffers()
{
	//---------- Vertex Buffer ----------//
	_stagingBuffer = new StagingBuffer(_device, _model->getVertices());
	_vertexBuffer = new VertexBuffer(_device, _model->getVertices());
	// Transfer from staging buffer to vertex buffer
	_vertexBuffer->copyFrom(_commandPool, _stagingBuffer->handle(), sizeof(_model->getVertices()[0])*_model->getVertices().size());
	delete _stagingBuffer;
	_stagingBuffer = nullptr;

	//---------- Index Buffer ----------//
	_stagingBuffer = new StagingBuffer(_device, _model->getIndices());
	_indexBuffer = new IndexBuffer(_device, _model->getIndices());
	// Transfer from staging buffer to vertex buffer
	_indexBuffer->copyFrom(_commandPool, _stagingBuffer->handle(), sizeof(_model->getIndices()[0])*_model->getIndices().size());
	delete _stagingBuffer;
	_stagingBuffer = nullptr;
	
	//---------- Uniform Buffers ----------//
	_uniformBuffers.resize(_swapChain->getImages().size());
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);
	for(size_t i = 0; i < _swapChain->getImages().size(); i++) 
	{
		_uniformBuffers[i] = new UniformBuffer(_device, bufferSize);
    }
}

void Application::updateUniformBuffer(uint32_t currentImage)
{
 	static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo{};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), _swapChain->getExtent().width / (float) _swapChain->getExtent().height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	void* data;
	vkMapMemory(_device->handle(), _uniformBuffers[currentImage]->getMemory(), 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(_device->handle(), _uniformBuffers[currentImage]->getMemory());
}
