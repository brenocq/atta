//--------------------------------------------------
// Robot Simulator
// application.cpp
// Date: 21/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "application.h"

Application::Application():
	_currentFrame(0), _framebufferResized(false)
{
	_window = new Window();
	_instance = new Instance();
	_debugMessenger = new DebugMessenger(_instance);
	_surface = new Surface(_instance, _window);
	_physicalDevice = new PhysicalDevice(_instance, _surface);
	_device = new Device(_physicalDevice);
	_commandPool = new CommandPool(_device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	_swapChain = new SwapChain(_device, _window);
	_descriptorSetLayout = new DescriptorSetLayout(_device);
	_colorBuffer = new ColorBuffer(_device, _swapChain, _swapChain->getExtent());
	_depthBuffer = new DepthBuffer(_device, _commandPool, _swapChain->getExtent());
	_graphicsPipeline = new GraphicsPipeline(_device, _swapChain, _depthBuffer, _colorBuffer, _descriptorSetLayout);

	_frameBuffers.resize(_swapChain->getImageViews().size());
	for(size_t i = 0; i < _frameBuffers.size(); i++) 
		_frameBuffers[i] = new FrameBuffer(_swapChain->getImageViews()[i], _graphicsPipeline->getRenderPass());

	std::vector<Model*> models;
	std::vector<Texture*> textures;

	models.push_back(new Model("cube_multi"));
	models.push_back(Model::createSphere(glm::vec3(1, 0, 0), 0.5, Material::metallic(glm::vec3(0.7f, 0.5f, 0.8f), 0.2f), true));
	models.push_back(Model::createSphere(glm::vec3(-1, 0, 0), 0.5, Material::dielectric(1.5f), true));
	models.push_back(Model::createSphere(glm::vec3(0, 1, 0), 0.5, Material::lambertian(glm::vec3(1.0f), 0), true));
	textures.push_back(new Texture(_device, _commandPool, "assets/models/cube/cube.png"));

	//_model = new Model("cube_multi");
	//_model->loadTexture(_device, _commandPool);

	_scene = new Scene(_commandPool, models, textures, true);
	createBuffers();

	createDescriptorPool();
	//_texture = new Texture(_device, _commandPool, "assets/textures/texture.jpg");
	_descriptorSets = new DescriptorSets(_device, _descriptorPool, _descriptorSetLayout, _uniformBuffers, textures[0]);

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

	delete _depthBuffer;
	_depthBuffer = nullptr;

	delete _scene;
	_scene = nullptr;

	delete _descriptorSetLayout;
	_descriptorSetLayout = nullptr;

	//delete _indexBuffer;
	//_indexBuffer = nullptr;
	//delete _vertexBuffer;
	//_vertexBuffer = nullptr;

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
	// TODO crashing application
	_window->waitIfMinimized();
	vkDeviceWaitIdle(_device->handle());

	cleanupSwapChain();

	_swapChain = new SwapChain(_device, _window);
	_colorBuffer = new ColorBuffer(_device, _swapChain, _swapChain->getExtent());
	_depthBuffer = new DepthBuffer(_device, _commandPool, _swapChain->getExtent());
	_graphicsPipeline = new GraphicsPipeline(_device, _swapChain, _depthBuffer, _colorBuffer, _descriptorSetLayout);
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

	createDescriptorPool();
	createCommandBuffers();

	// IMGUI
	_userInterface = new UserInterface(_device, _window, _swapChain);
	_rayTracing->createSwapChain();
}

void Application::run()
{
	_window->drawFrame = [this](){ drawFrame(); };
	_window->windowResized = [this](){ framebufferResizeCallback(); };
	_window->loop();
	vkDeviceWaitIdle(_device->handle());
}

void Application::drawFrame()
{
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
	_rayTracing->render(_commandBuffersTest[imageIndex], imageIndex);
	//render(imageIndex);
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
    { _commandBuffersTest[imageIndex],  _userInterface->getCommandBuffer(imageIndex)};

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

	_currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Application::render(int i)
{
	//---------- Start command pool ----------//
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	//beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	if(vkBeginCommandBuffer(_commandBuffersTest[i], &beginInfo) != VK_SUCCESS)
	{
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
	clearValues[0].color = {0.5f, 0.5f, 0.5f, 1.0f};
	clearValues[1].depthStencil = {1.0f, 0};
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(_commandBuffersTest[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	{
		vkCmdBindPipeline(_commandBuffersTest[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline->handle());

		//VkBuffer vertexBuffers[] = {_vertexBuffer->handle()};
		//VkDeviceSize offsets[] = {0};
		//vkCmdBindVertexBuffers(_commandBuffersTest[i], 0, 1, vertexBuffers, offsets);
		//vkCmdBindIndexBuffer(_commandBuffersTest[i], _indexBuffer->handle(), 0, VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(_commandBuffersTest[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline->getPipelineLayout()->handle(), 0, 1, &_descriptorSets->handle()[i], 0, nullptr);

		uint32_t vertexOffset = 0;
		uint32_t indexOffset = 0;

		for (auto model : _scene->getModels())
		{
			const uint32_t vertexCount = static_cast<uint32_t>(model->getVertices().size());
			const uint32_t indexCount = static_cast<uint32_t>(model->getIndices().size());

			vkCmdDrawIndexed(_commandBuffersTest[i], indexCount, 1, indexOffset, vertexOffset, 0);

			vertexOffset += vertexCount;
			indexOffset += indexCount;
		}
	}
	vkCmdEndRenderPass(_commandBuffersTest[i]);

	vkEndCommandBuffer(_commandBuffersTest[i]);
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

	if(vkAllocateCommandBuffers(_device->handle(), &allocInfo, _commandBuffersTest.data()) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[Application]" << RESET << RED << " Failed to allocate command buffers!" << RESET << std::endl;
		exit(1);
	}

	//for(size_t i = 0; i < _commandBuffersTest.size(); i++)
	//{
	//	VkCommandBufferBeginInfo beginInfo{};
	//	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	//	if(vkBeginCommandBuffer(_commandBuffersTest[i], &beginInfo) != VK_SUCCESS)
	//	{
	//		std::cout << BOLDRED << "[CommandBuffers]" << RESET << RED << " Failed to begin recording command buffer!" << RESET << std::endl;
	//		exit(1);
	//	}

	//	VkRenderPassBeginInfo renderPassInfo{};
	//	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	//	renderPassInfo.renderPass = _graphicsPipeline->getRenderPass()->handle();
	//	renderPassInfo.framebuffer = _frameBuffers[i]->handle();
	//	renderPassInfo.renderArea.offset = {0, 0};
	//	renderPassInfo.renderArea.extent = _swapChain->getExtent();

	//	std::array<VkClearValue, 2> clearValues{};
	//	clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
	//	clearValues[1].depthStencil = {1.0f, 0};
	//	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	//	renderPassInfo.pClearValues = clearValues.data();

	//	vkCmdBeginRenderPass(_commandBuffersTest[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	//		vkCmdBindPipeline(_commandBuffersTest[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline->handle());

	//		VkBuffer vertexBuffers[] = {_vertexBuffer->handle()};
	//		VkDeviceSize offsets[] = {0};
	//		vkCmdBindVertexBuffers(_commandBuffersTest[i], 0, 1, vertexBuffers, offsets);
	//		vkCmdBindIndexBuffer(_commandBuffersTest[i], _indexBuffer->handle(), 0, VK_INDEX_TYPE_UINT32);

	//		vkCmdBindDescriptorSets(_commandBuffersTest[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline->getPipelineLayout()->handle(), 0, 1, &_descriptorSets->handle()[i], 0, nullptr);
	//		vkCmdDrawIndexed(_commandBuffersTest[i], static_cast<uint32_t>(_model->getIndices().size()), 1, 0, 0, 0);

	//	vkCmdEndRenderPass(_commandBuffersTest[i]);

	//	if (vkEndCommandBuffer(_commandBuffersTest[i]) != VK_SUCCESS) {
	//		std::cout << BOLDRED << "[CommandBuffers]" << RESET << RED << " Failed to record command buffer!" << RESET << std::endl;
	//		exit(1);
	//	}
	//}
}

void Application::createBuffers()
{
	//---------- Vertex Buffer ----------//
	//_stagingBuffer = new StagingBuffer(_device, _model->getVertices());
	//_vertexBuffer = new VertexBuffer(_device, _model->getVertices());
	//// Transfer from staging buffer to vertex buffer
	//_vertexBuffer->copyFrom(_commandPool, _stagingBuffer->handle(), sizeof(_model->getVertices()[0])*_model->getVertices().size());
	//delete _stagingBuffer;
	//_stagingBuffer = nullptr;

	////---------- Index Buffer ----------//
	//_stagingBuffer = new StagingBuffer(_device, _model->getIndices());
	//_indexBuffer = new IndexBuffer(_device, _model->getIndices());
	//// Transfer from staging buffer to vertex buffer
	//_indexBuffer->copyFrom(_commandPool, _stagingBuffer->handle(), sizeof(_model->getIndices()[0])*_model->getIndices().size());
	//delete _stagingBuffer;
	//_stagingBuffer = nullptr;
	
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
 	//static auto startTime = std::chrono::high_resolution_clock::now();

    //auto currentTime = std::chrono::high_resolution_clock::now();
    //float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	//UniformBufferObject ubo{};
	//ubo.modelView = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//ubo.projection = glm::perspective(glm::radians(45.0f), _swapChain->getExtent().width / (float) _swapChain->getExtent().height, 0.1f, 10.0f);
	//ubo.projection[1][1] *= -1;

	//void* data;
	//vkMapMemory(_device->handle(), _uniformBuffers[currentImage]->getMemory(), 0, sizeof(ubo), 0, &data);
	//	memcpy(data, &ubo, sizeof(ubo));
	//vkUnmapMemory(_device->handle(), _uniformBuffers[currentImage]->getMemory());
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
