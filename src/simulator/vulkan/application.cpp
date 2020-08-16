//--------------------------------------------------
// Robot Simulator
// application.cpp
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "application.h"

Application::Application(Scene* scene):
	_scene(scene), _currentFrame(0), _framebufferResized(false)
{
	_window = new Window();
	_instance = new Instance();
	_debugMessenger = new DebugMessenger(_instance);
	_surface = new Surface(_instance, _window);
	_physicalDevice = new PhysicalDevice(_instance, _surface);
	_device = new Device(_physicalDevice);
	_commandPool = new CommandPool(_device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

	//---------- Scene ----------//
	_scene->createBuffers(_commandPool);

	//---------- Swap Chain ----------//
	_swapChain = new SwapChain(_device, _window);
	
	//---------- Uniform Buffers ----------//
	_uniformBuffers.resize(_swapChain->getImages().size());
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);
	for(size_t i = 0; i < _swapChain->getImages().size(); i++) 
	{
		_uniformBuffers[i] = new UniformBuffer(_device, bufferSize);
    }

	//---------- Graphics pipeline ----------//
	createPipelines();

	//---------- Frame Buffers ----------//
	_frameBuffers.resize(_swapChain->getImageViews().size());
	for(size_t i = 0; i < _frameBuffers.size(); i++) 
		_frameBuffers[i] = new FrameBuffer(_swapChain->getImageViews()[i], _renderPass);

	createDescriptorPool();
	_commandBuffers = new CommandBuffers(_device, _commandPool, _frameBuffers.size());

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

	// Model view controller
	_modelViewController = new ModelViewController(_window);
	_modelViewController->reset(glm::lookAt(glm::vec3(2, 2, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));

	// IMGUI
	_userInterface = new UserInterface(_device, _window, _swapChain);

	// RayTracing
	_rayTracing = new RayTracing(_device, _swapChain, _commandPool, _uniformBuffers, _scene);
}

Application::~Application()
{
	cleanupSwapChain();

	delete _rayTracing;
	_rayTracing = nullptr;

	delete _userInterface;
	_userInterface = nullptr;

	delete _modelViewController;
	_modelViewController = nullptr;

	delete _scene;
	_scene = nullptr;

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

void Application::createPipelines()
{
	_colorBuffer = new ColorBuffer(_device, _swapChain, _swapChain->getExtent());
	_depthBuffer = new DepthBuffer(_device, _commandPool, _swapChain->getExtent());
	_renderPass = new RenderPass(_device, _swapChain, _depthBuffer, _colorBuffer);
	_graphicsPipeline = new GraphicsPipeline(_device, _swapChain, _renderPass, _uniformBuffers, _scene);
	_linePipeline = new LinePipeline(_device, _swapChain, _renderPass, _uniformBuffers, _scene);
}

void Application::cleanupSwapChain()
{
	_rayTracing->deleteSwapChain();

	delete _userInterface;

	_userInterface = nullptr;

	delete _colorBuffer;
	_colorBuffer = nullptr;

	delete _depthBuffer;
	_depthBuffer = nullptr;

	for (auto frameBuffer : _frameBuffers) 
	{
		delete frameBuffer;
		frameBuffer = nullptr;
    }

	delete _commandBuffers;
	_commandBuffers = nullptr;

	delete _linePipeline;
	_linePipeline = nullptr;

	delete _graphicsPipeline;
	_graphicsPipeline = nullptr;

	delete _renderPass;
	_renderPass = nullptr;

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
	// TODO crashing application
	_window->waitIfMinimized();
	vkDeviceWaitIdle(_device->handle());

	cleanupSwapChain();

	_swapChain = new SwapChain(_device, _window);

	_uniformBuffers.resize(_swapChain->getImages().size());
	for(size_t i = 0; i < _swapChain->getImages().size(); i++) 
	{
		_uniformBuffers[i] = new UniformBuffer(_device, (int)sizeof(UniformBufferObject));
    }

	createPipelines();
	_frameBuffers.resize(_swapChain->getImageViews().size());

	for(size_t i = 0; i < _frameBuffers.size(); i++) 
	{
		_frameBuffers[i] = new FrameBuffer(_swapChain->getImageViews()[i], _renderPass);
	}

	createDescriptorPool();
	_commandBuffers = new CommandBuffers(_device, _commandPool, _frameBuffers.size());

	// IMGUI
	_userInterface = new UserInterface(_device, _window, _swapChain);
	_rayTracing->createSwapChain();
}

void Application::run()
{
	_window->drawFrame = [this](){ drawFrame(); };
	_window->windowResized = [this](){ framebufferResizeCallback(); };
	_window->onKey = [this](const int key, const int scancode, const int action, const int mods) { onKey(key, scancode, action, mods); };
	_window->onCursorPosition = [this](const double xpos, const double ypos) { onCursorPosition(xpos, ypos); };
	_window->onMouseButton = [this](const int button, const int action, const int mods) { onMouseButton(button, action, mods); };
	_window->onScroll = [this](const double xoffset, const double yoffset) { onScroll(xoffset, yoffset); };
	_window->loop();
	vkDeviceWaitIdle(_device->handle());
}

void Application::drawFrame()
{
	//---------- Time ----------//
	const double prevTime = _time;
	_time = _window->getTime();
	const double timeDelta = _time - prevTime;

	if(onDrawFrame)
		onDrawFrame(timeDelta);

	
	bool cameraUpdated = _modelViewController->updateCamera(timeDelta);

	//-----------------------------//
	//---------- Drawing ----------//
	//-----------------------------//
	uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(_device->handle(), _swapChain->handle(), UINT64_MAX, _imageAvailableSemaphores[_currentFrame]->handle(), VK_NULL_HANDLE, &imageIndex);

	//---------- Check swapchain ----------//
	if(result == VK_ERROR_OUT_OF_DATE_KHR) 
	{
		// Recreate the swapchain (window resized)
		recreateSwapChain();
		return;
	} 
	else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) 
	{
		std::cout << BOLDRED << "[Application]" << RESET << RED << " Failed to acquire swap chain image!" << RESET << std::endl;
		exit(1);
	}
	//----------- Render to screen ----------//
	//_rayTracing->render(_commandBuffers->handle()[imageIndex], imageIndex);
	render(imageIndex);
	_userInterface->render(imageIndex);

	//---------- CPU-GPU syncronization ----------//
	_inFlightFences[_currentFrame]->wait(UINT64_MAX);
	_inFlightFences[_currentFrame]->reset();

	//---------- Submit to queues ----------//
	updateUniformBuffer(imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {_imageAvailableSemaphores[_currentFrame]->handle()};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	VkSemaphore signalSemaphores[] = {_renderFinishedSemaphores[_currentFrame]->handle()};

	std::array<VkCommandBuffer, 2> submitCommandBuffers =
    { _commandBuffers->handle()[imageIndex],  _userInterface->getCommandBuffer(imageIndex)};

	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = static_cast<uint32_t>(submitCommandBuffers.size());
	submitInfo.pCommandBuffers = submitCommandBuffers.data();
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	_inFlightFences[_currentFrame]->reset();

	if (vkQueueSubmit(_device->getGraphicsQueue(), 1, &submitInfo, _inFlightFences[_currentFrame]->handle()) != VK_SUCCESS) {
		std::cout << BOLDRED << "[Application]" << RESET << RED << " Failed to submit draw command buffer!" << RESET << std::endl;
		exit(1);
	}

	//---------- Present ----------//
	VkSwapchainKHR swapChains[] = {_swapChain->handle()};
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
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

	_currentFrame = (_currentFrame + 1) % _inFlightFences.size();
}

void Application::render(int i)
{
	VkCommandBuffer commandBuffer = _commandBuffers->handle()[i];
	//---------- Start command pool ----------//
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	//beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[CommandBuffers]" << RESET << RED << " Failed to begin recording command buffer!" << RESET << std::endl;
		exit(1);
	}

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = {0.3f, 0.3f, 0.3f, 1.0f};
	clearValues[1].depthStencil = {1.0f, 0};

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = _renderPass->handle();
	renderPassInfo.framebuffer = _frameBuffers[i]->handle();
	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = _swapChain->getExtent();
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	{
		VkBuffer vertexBuffers[] = { _scene->getVertexBuffer()->handle() };
		const VkBuffer indexBuffer = _scene->getIndexBuffer()->handle();
		VkDeviceSize offsets[] = { 0 };

		// Graphics pipeline
		{
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline->handle());
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline->getPipelineLayout()->handle(), 0, 1, &_graphicsPipeline->getDescriptorSets()->handle()[i], 0, nullptr);
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
			vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

			for(auto object : _scene->getObjects())
			{
				Model* model = object->getModel();

				ObjectInfo objectInfo;
				objectInfo.modelMatrix = object->getModelMat();

				vkCmdPushConstants(
						commandBuffer,
						_graphicsPipeline->getPipelineLayout()->handle(),
						VK_SHADER_STAGE_VERTEX_BIT,
						0,
						sizeof(ObjectInfo),
						&objectInfo);

				const uint32_t vertexCount = model->getVerticesSize();
				const uint32_t indexCount = model->getIndicesSize();
				const uint32_t vertexOffset = model->getVertexOffset();
				const uint32_t indexOffset = model->getIndexOffset();

				vkCmdDrawIndexed(commandBuffer, indexCount, 1, indexOffset, vertexOffset, 0);
			}
		}

		// Line pipeline
		{
			VkBuffer lineVertexBuffers[] = { _scene->getLineVertexBuffer()->handle() };
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _linePipeline->handle());
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _linePipeline->getPipelineLayout()->handle(), 0, 1, &_linePipeline->getDescriptorSets()->handle()[i], 0, nullptr);
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, lineVertexBuffers, offsets);
			vkCmdBindIndexBuffer(commandBuffer, _scene->getLineIndexBuffer()->handle(), 0, VK_INDEX_TYPE_UINT32);


			ObjectInfo objectInfo;
			objectInfo.modelMatrix = glm::mat4(1);

			vkCmdPushConstants(
					commandBuffer,
					_graphicsPipeline->getPipelineLayout()->handle(),
					VK_SHADER_STAGE_VERTEX_BIT,
					0,
					sizeof(ObjectInfo),
					&objectInfo);

			const uint32_t indexCount = static_cast<uint32_t>(_scene->getLineIndexCount());
			vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
		}
	}
	vkCmdEndRenderPass(commandBuffer);

	vkEndCommandBuffer(commandBuffer);
}

void Application::updateUniformBuffer(uint32_t currentImage)
{
	static int test=0;

	UniformBufferObject ubo;
	ubo.modelView = _modelViewController->getModelView();
	ubo.aperture = 0.02f;
	ubo.focusDistance = 2.0f;
	ubo.projection = glm::perspective(glm::radians(90.0f), _swapChain->getExtent().width / static_cast<float>(_swapChain->getExtent().height), 0.1f, 10000.0f);
	ubo.projection[1][1] *= -1; // Inverting Y for Vulkan, https://matthewwellings.com/blog/the-new-vulkan-coordinate-system/
	ubo.modelViewInverse = glm::inverse(ubo.modelView);
	ubo.projectionInverse = glm::inverse(ubo.projection);
	ubo.totalNumberOfSamples = test++>100?100:100-test;
	ubo.numberOfSamples = test++>100?100:100-test;
	ubo.numberOfBounces = 9;
	ubo.randomSeed = 1;
	ubo.gammaCorrection = true;
	ubo.hasSky = false;

	_uniformBuffers[currentImage]->setValue(ubo);
}

void Application::createDescriptorPool()
{
	int size = _swapChain->getImages().size();

	std::vector<VkDescriptorPoolSize> poolSizes{};
	VkDescriptorPoolSize uniformBufferDescriptor;
	VkDescriptorPoolSize combinedImageSamplerDescriptor;
	uniformBufferDescriptor.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uniformBufferDescriptor.descriptorCount = static_cast<uint32_t>(size);
	combinedImageSamplerDescriptor.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	combinedImageSamplerDescriptor.descriptorCount = static_cast<uint32_t>(size);
	poolSizes.push_back(uniformBufferDescriptor);
	poolSizes.push_back(combinedImageSamplerDescriptor);

	_descriptorPool = new DescriptorPool(_device, poolSizes);
}

void Application::onKey(int key, int scancode, int action, int mods)
{
	switch(key)
	{
		case GLFW_KEY_ESCAPE:
			_window->close();
			break;
		case GLFW_KEY_F1:
			if(action == GLFW_PRESS)
				_window->toggleCursorVisibility();
			break;
	}
	_modelViewController->onKey(key, scancode, action, mods);
}

void Application::onCursorPosition(double xpos, double ypos)
{

	_modelViewController->onCursorPosition(xpos, ypos);
}

void Application::onMouseButton(int button, int action, int mods)
{
	_modelViewController->onMouseButton(button, action, mods);
}

void Application::onScroll(double xoffset, double yoffset)
{
	_modelViewController->onScroll(xoffset, yoffset);
}
