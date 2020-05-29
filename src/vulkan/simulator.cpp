#include "simulator.h"

Simulator::Simulator()
{
	_isWireFrame = false;
	_totalNumberOfSamples = 0;

	initWindow();
	initInstance();
	_debugUtilsMessenger = ENABLE_VALIDATION_LAYERS ? new DebugUtilsMessenger(_instance, VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) : nullptr;
	initSurface();
	initDevice();

	// Test scene
	std::vector<Model> models;
	std::vector<Texture> textures;

	models.push_back(Model::loadModel("assets/models/cube_multi.obj"));
	models.push_back(Model::createSphere(glm::vec3(1, 0, 0), 0.5, Material::Metallic(glm::vec3(0.7f, 0.5f, 0.8f), 0.2f), true));
	models.push_back(Model::createSphere(glm::vec3(-1, 0, 0), 0.5, Material::Dielectric(1.5f), true));
	models.push_back(Model::createSphere(glm::vec3(0, 1, 0), 0.5, Material::Lambertian(glm::vec3(1.0f), 0), true));

	textures.push_back(Texture::loadTexture("assets/textures/land_ocean_ice_cloud_2048.png"));

	_scene = new Scene(_commandPool, std::move(models), std::move(textures), false);

	initSwapChain();
	checkFramebufferSize();
}

Simulator::~Simulator()
{
	delete _commandPool;
	delete _device;
	delete _instance;
	delete _window;
	delete _scene;
	delete _camera;
	delete _debugUtilsMessenger;
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

	CameraInitialState cameraState;
	cameraState.ModelView = glm::translate(glm::mat4(1), glm::vec3(0, 0, -2));
	cameraState.FieldOfView = 90;
	cameraState.Aperture = 0.05f;
	cameraState.FocusDistance = 2.0f;
	cameraState.ControlSpeed = 2.0f;
	cameraState.GammaCorrection = false;
	cameraState.HasSky = true;

	_camera = new Camera(cameraState);
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
	_commandPool = new CommandPool(_device, _device->graphicsFamilyIndex(), true);
}

void Simulator::initSwapChain()
{
	// Wait until the window is visible.
	while (_window->isMinimized())
	{
		_window->waitForEvents();
	}

	_swapChain = new SwapChain(_device);
	_depthBuffer = new DepthBuffer(_commandPool, _swapChain->extent());

	for (size_t i = 0; i != _swapChain->imageViews().size(); i++)
	{
		_imageAvailableSemaphores.emplace_back(_device);
		_renderFinishedSemaphores.emplace_back(_device);
		_inFlightFences.emplace_back(_device, true);
		_uniformBuffers.emplace_back(_device);
	}

	_graphicsPipeline = new GraphicsPipeline(_swapChain, _depthBuffer, _uniformBuffers, _scene, false);

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
	_device->waitIdle();
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

	if (result == VK_ERROR_OUT_OF_DATE_KHR /*|| _isWireFrame!= _graphicsPipeline->isWireFrame()*/)
	{
		recreateSwapChain();
		return;
	}

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		//Throw(std::runtime_error(std::string("failed to acquire next image (") + ToString(result) + ")"));
		std::cout << BOLDRED << "[Simulator] Failed to acquire next image!" << RESET << std::endl;
		exit(1);
	}

	const auto commandBuffer = _commandBuffers->begin(imageIndex);
	render(commandBuffer, imageIndex);
	_commandBuffers->end(imageIndex);

	// Update uniform buffer
	_uniformBuffers[imageIndex].setValue(getUniformBufferObject(_swapChain->extent()));

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
		recreateSwapChain();
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
	std::array<VkClearValue, 2> clearValues = {};
	clearValues[0].color = { {0.3f, 0.6f, 0.4f, 1.0f} };
	clearValues[1].depthStencil = { 1.0f, 0 };

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
		VkDescriptorSet descriptorSets[] = { _graphicsPipeline->descriptorSet(imageIndex) };
		VkBuffer vertexBuffers[] = { _scene->vertexBuffer()->handle() };
		const VkBuffer indexBuffer = _scene->indexBuffer()->handle();
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline->handle());
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline->pipelineLayout()->handle(), 0, 1, descriptorSets, 0, nullptr);
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

		uint32_t vertexOffset = 0;
		uint32_t indexOffset = 0;
		
		for (const auto& model : _scene->models())
		{
			const auto vertexCount = static_cast<uint32_t>(model.numberOfVertices());
			const auto indexCount = static_cast<uint32_t>(model.numberOfIndices());

			vkCmdDrawIndexed(commandBuffer, indexCount, 1, indexOffset, vertexOffset, 0);

			vertexOffset += vertexCount;
			indexOffset += indexCount;
		}
	}
	vkCmdEndRenderPass(commandBuffer);
}

UniformBufferObject Simulator::getUniformBufferObject(VkExtent2D extent)
{
	const auto& init = _camera->initialState();
	float fieldOfView = 20.0;

	UniformBufferObject ubo = {};
	ubo.modelView = _camera->modelView();
	ubo.projection = glm::perspective(glm::radians(fieldOfView), extent.width / static_cast<float>(extent.height), 0.1f, 10000.0f);
	ubo.projection[1][1] *= -1; // Inverting Y for Vulkan, https://matthewwellings.com/blog/the-new-vulkan-coordinate-system/
	ubo.modelViewInverse = glm::inverse(ubo.modelView);
	ubo.projectionInverse = glm::inverse(ubo.projection);
	ubo.aperture = 0.1;
	ubo.focusDistance = 10;
	ubo.totalNumberOfSamples = _totalNumberOfSamples;
	ubo.numberOfSamples = 8;
	ubo.numberOfBounces = 5;
	ubo.randomSeed = 1;
	ubo.gammaCorrection = true;
	ubo.hasSky = true;

	return ubo;
}

void Simulator::recreateSwapChain()
{
	_device->waitIdle();

	// Delete swapchain
	delete _commandBuffers;
	_swapChainFramebuffers.clear();
	delete _graphicsPipeline;
	_uniformBuffers.clear();
	_inFlightFences.clear();
	_renderFinishedSemaphores.clear();
	_imageAvailableSemaphores.clear();
	delete _depthBuffer;
	delete _swapChain;

	// Create swapchain
	initSwapChain();
}

void Simulator::checkFramebufferSize() const
{
	// Check the framebuffer size when requesting a fullscreen window, as it's not guaranteed to match.
	const auto& cfg = _window->config();
	const auto fbSize = _window->framebufferSize();
	
	if (cfg.fullscreen && (fbSize.width != cfg.width || fbSize.height != cfg.height))
	{
		std::ostringstream out;
		out << "framebuffer fullscreen size mismatch (requested: ";
		out << cfg.width << "x" << cfg.height;
		out << ", got: ";
		out << fbSize.width << "x" << fbSize.height << ")";
		
		std::cout << out.str() << std::endl;
		exit(1);
	}
}
