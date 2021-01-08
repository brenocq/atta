//--------------------------------------------------
// Robot Simulator
// application.cpp
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "application.h"
#include "simulator/objects/basics/basics.h"
#include "simulator/objects/others/display/display.h"
#include "simulator/objects/sensors/camera/camera.h"
#include "simulator/physics/physicsEngine.h"
#include "simulator/helpers/log.h"

Application::Application(std::shared_ptr<Window> window, std::shared_ptr<Scene> scene, RenderingType renderingType):
	_window(window), _scene(scene), _renderingType(renderingType),
	_currentFrame(0), _framebufferResized(false), _enableRayTracing(false), _totalNumberOfSamples(0), _splitRender(false),
	_rayTracing(nullptr)
{
	_instance = std::make_shared<Instance>();
	_debugMessenger = std::make_unique<DebugMessenger>(_instance);
	_surface = std::make_shared<Surface>(_instance, _window);
	_physicalDevice = std::make_shared<PhysicalDevice>(_instance, _surface);
	_device = std::make_shared<Device>(_physicalDevice);
	_commandPool = std::make_shared<CommandPool>(_device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

	//---------- Scene ----------//
	//_scene->createBuffers(_commandPool);

	////---------- Swap Chain ----------//
	//_swapChain = std::make_shared<SwapChain>(_device, _window);
	//
	////---------- Uniform Buffers ----------//
	//_uniformBuffers.resize(_swapChain->getImages().size());
	//VkDeviceSize bufferSize = sizeof(UniformBufferObject);
	//for(size_t i = 0; i < _swapChain->getImages().size(); i++) 
	//{
	//	_uniformBuffers[i] = new UniformBuffer(_device, bufferSize);
    //}

	////---------- Pipelines ----------//
	//createPipelines();

	////---------- Command buffers ----------//
	//_commandBuffers = std::make_unique<CommandBuffers>(_device, _commandPool, _swapChain->getImageViews().size());

	////---------- Syncronization ----------//
	//_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	//_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	//_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	//_imagesInFlight.resize(_swapChain->getImages().size(), VK_NULL_HANDLE);

	//for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
	//{
	//	_imageAvailableSemaphores[i] = new Semaphore(_device);
	//	_renderFinishedSemaphores[i] = new Semaphore(_device);
	//	_inFlightFences[i] = new Fence(_device);
	//}

	////---------- Model view controller ----------//
	//_modelViewController = new ModelViewController(_window);
	//_modelViewController->reset(glm::lookAt(glm::vec3(3, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));

	////---------- User Interface ----------//
	//createUserInterface();

	////---------- RayTracing ----------//
	//if(_device->getRayTracingEnabled())
	//{
	//	_rayTracing = new RayTracing(_device, _swapChain, _commandPool, _uniformBuffers, _scene);
	//}
	//_rayTracingCPU = new atta::rt::RayTracingCPU();

	////---------- Scene special objects ----------//
	//createSpecialSceneObjects();
}

Application::~Application()
{
	//cleanupSwapChain();

	//for(auto& rayTracing : _camerasRayTracing)
	//{
	//	if(rayTracing!=nullptr)
	//	{
	//		delete rayTracing;
	//		rayTracing = nullptr;
	//	}
	//}

	//for(auto& pipeline : _camerasRasterization) 
	//{
	//	if(pipeline!=nullptr)
	//	{
	//		delete pipeline;
	//		pipeline = nullptr;
	//	}
    //}

	//for(auto& imageView : _camerasRasterizationImageView) 
	//{
	//	if(imageView!=nullptr)
	//	{
	//		delete imageView;
	//		imageView = nullptr;
	//	}
    //}

	//for(auto& image : _camerasRasterizationImage) 
	//{
	//	if(image!=nullptr)
	//	{
	//		delete image;
	//		image = nullptr;
	//	}
    //}

	//for(auto& uniformBuffer : _camerasUniformBuffer) 
	//{
	//	delete uniformBuffer;
	//	uniformBuffer = nullptr;
    //}

	//if(_rayTracing != nullptr)
	//{
	//	delete _rayTracing;
	//	_rayTracing = nullptr;
	//}

	//if(_rayTracingCPU != nullptr)
	//{
	//	delete _rayTracingCPU;
	//	_rayTracingCPU = nullptr;
	//}

	////delete _userInterface;
	////_userInterface = nullptr;

	//delete _modelViewController;
	//_modelViewController = nullptr;

	//delete _scene;
	//_scene = nullptr;

	//for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
	//{
	//	delete _renderFinishedSemaphores[i];
	//	_renderFinishedSemaphores[i] = nullptr;
	//	delete _imageAvailableSemaphores[i];
	//	_imageAvailableSemaphores[i] = nullptr;
	//	delete _inFlightFences[i];
	//	_inFlightFences[i] = nullptr;
    //}

	//delete _commandPool;
	//_commandPool = nullptr;

	//delete _device;
	//_device = nullptr;

	//delete _debugMessenger;
	//_debugMessenger = nullptr;

	//delete _surface;
	//_surface = nullptr;

	//delete _window;
	//_window = nullptr;

	//delete _physicalDevice;
	//_physicalDevice = nullptr;
}

void Application::createPipelines()
{
	//_graphicsPipeline = std::make_unique<GraphicsPipeline>(_device, _swapChain, _uniformBuffers, _scene);
	//_linePipeline = std::make_unique<LinePipeline>(_device, _swapChain, _graphicsPipeline->getRenderPass(), _uniformBuffers, _scene);
	//_maskPipeline = std::make_unique<MaskPipeline>(_device, _swapChain, _graphicsPipeline->getRenderPass(), _uniformBuffers, _scene);
	//_outlinePipeline = std::make_unique<OutlinePipeline>(_device, _swapChain, _graphicsPipeline->getRenderPass(), _uniformBuffers, _scene);
	//_skyboxPipeline = std::make_unique<SkyboxPipeline>(_device, _swapChain, _graphicsPipeline->getRenderPass(), _uniformBuffers, _scene);
}

void Application::createUserInterface()
{
	//_userInterface.reset();
	//_userInterface = std::make_unique<UserInterface>(_device, _window, _swapChain, _scene);
	//_userInterface->setEnableRayTacing(&_enableRayTracing);
	//_userInterface->setSplitRender(&_splitRender);
}

void Application::cleanupSwapChain()
{
	//if(_rayTracing != nullptr)
	//	_rayTracing->deletePipeline();

	//delete _userInterface;
	//_userInterface = nullptr;

	//delete _commandBuffers;
	//_commandBuffers = nullptr;

	//delete _maskPipeline;
	//_maskPipeline = nullptr;

	//delete _outlinePipeline;
	//_outlinePipeline = nullptr;

	//delete _skyboxPipeline;
	//_skyboxPipeline = nullptr;

	//delete _linePipeline;
	//_linePipeline = nullptr;

	//delete _graphicsPipeline;
	//_graphicsPipeline = nullptr;

	//for(auto& uniformBuffer : _uniformBuffers) 
	//{
	//	delete uniformBuffer;
	//	uniformBuffer = nullptr;
    //}
}

void Application::recreateSwapChain()
{
	//// TODO crashing application
	//_window->waitIfMinimized();
	//vkDeviceWaitIdle(_device->handle());

	//cleanupSwapChain();

	//_swapChain = std::make_shared<SwapChain>(_device, _window);


	//_uniformBuffers.resize(_swapChain->getImages().size());
	//for(size_t i = 0; i < _swapChain->getImages().size(); i++) 
	//{
	//	_uniformBuffers[i] = new UniformBuffer(_device, (int)sizeof(UniformBufferObject));
    //}

	//createPipelines();
	////_frameBuffers.resize(_swapChain->getImageViews().size());

	////for(size_t i = 0; i < _frameBuffers.size(); i++) 
	////{
	////	_frameBuffers[i] = new FrameBuffer(_swapChain->getImageViews()[i], _graphicsPipeline->getRenderPass());
	////}
	//_commandBuffers.reset();
	//_commandBuffers = std::make_unique<CommandBuffers>(_device, _commandPool, _swapChain->getImageViews().size());

	//// IMGUI
	//createUserInterface();
	//if(_rayTracing!=nullptr)
	//	_rayTracing->createPipeline();
}

void Application::run()
{
	//_window->drawFrame = [this](){ drawFrame(); };
	//_window->windowResized = [this](){ framebufferResizeCallback(); };
	//_window->onKey = [this](const int key, const int scancode, const int action, const int mods) { onKey(key, scancode, action, mods); };
	//_window->onCursorPosition = [this](const double xpos, const double ypos) { onCursorPosition(xpos, ypos); };
	//_window->onMouseButton = [this](const int button, const int action, const int mods) { onMouseButton(button, action, mods); };
	//_window->onScroll = [this](const double xoffset, const double yoffset) { onScroll(xoffset, yoffset); };
	//_window->loop();
	//vkDeviceWaitIdle(_device->handle());
}

void Application::drawFrame()
{
	//---------- Time ----------//
	const double prevTime = _time;
	_time = _window->getTime();
	const double timeDelta = _time - prevTime;

	//---------- Show from interface ----------//
	// Clean lines every frame
	_scene->cleanLines();
	// Show physics debugger
	//if(_userInterface->getShowPhysicsDebugger())
	//	_scene->getPhysicsEngine()->getWorld()->debugDrawWorld();

	if(onDrawFrame)
		onDrawFrame(timeDelta);

	// Update line buffer
	_scene->updateLineBuffer();
	// Update physics
	_scene->updatePhysics(0);

	_modelViewController->updateCamera(timeDelta);

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
		Log::error("Application", "Failed to acquire swap chain image!");
		exit(1);
	}

	//---------- Start recording to command buffer ----------//
	VkCommandBuffer commandBuffer = _commandBuffers->begin(imageIndex);
	{
		if(_enableRayTracing || _splitRender)
		{
			// Recreate raytracing swapChain
			_totalNumberOfSamples = 0;
			if(_rayTracing!=nullptr)
			{
				_rayTracing->recreateTopLevelStructures();
				_rayTracing->render(commandBuffer, imageIndex, _splitRender);
			}
		}

		if(!_enableRayTracing || _splitRender)
		{
			render(commandBuffer, imageIndex);
		}

		// Render raytracing cameras
		for(auto& rt : _camerasRayTracing)
		{
			rt->recreateTopLevelStructures();
			rt->render(commandBuffer);
		}

		// Render rasterization cameras
		for(auto& rast : _camerasRasterization)
		{
			rast->beginRender(commandBuffer);
			rast->render(commandBuffer);
			rast->endRender(commandBuffer);
		}
	}
	_commandBuffers->end(imageIndex);

	// Record to user interface command buffer
	_userInterface->render(imageIndex);

	//---------- CPU-GPU syncronization ----------//
	_inFlightFences[_currentFrame]->wait(UINT64_MAX);
	_inFlightFences[_currentFrame]->reset();

	//---------- GPU-GPU syncronization ----------//
	VkSemaphore waitSemaphores[] = {_imageAvailableSemaphores[_currentFrame]->handle()};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	VkSemaphore signalSemaphores[] = {_renderFinishedSemaphores[_currentFrame]->handle()};

	//---------- Update uniform buffers ----------//
	updateUniformBuffer(imageIndex);

	//---------- Submit to graphics queue ----------//
	std::array<VkCommandBuffer, 2> submitCommandBuffers =
    { _commandBuffers->handle()[imageIndex],  _userInterface->getCommandBuffer(imageIndex)};

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;// Wait image available
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = static_cast<uint32_t>(submitCommandBuffers.size());
	submitInfo.pCommandBuffers = submitCommandBuffers.data();
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	_inFlightFences[_currentFrame]->reset();

	if(vkQueueSubmit(_device->getGraphicsQueue(), 1, &submitInfo, _inFlightFences[_currentFrame]->handle()) != VK_SUCCESS)
	{
		Log::error("Application", "Failed to submit draw command buffer!");
		exit(1);
	}

	//---------- Submit do present queue ----------//
	VkSwapchainKHR swapChains[] = {_swapChain->handle()};
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;// Wait processing image
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
		Log::error("Application", "Failed to present swap chain image!");
		exit(1);
	}

	// Update special objects (camera, display, ...)
	//updateSpecialSceneObjects();

	//---------- Next frame ----------//
	_currentFrame = (_currentFrame + 1) % _inFlightFences.size();
}

void Application::render(VkCommandBuffer commandBuffer, int imageIndex)
{
	_graphicsPipeline->beginRender(commandBuffer, imageIndex);
	{
		// Skybox pipeline
		_skyboxPipeline->render(commandBuffer, imageIndex);

		// Graphics pipeline
		_graphicsPipeline->render(commandBuffer, imageIndex);

		// Line pipeline
		_linePipeline->render(commandBuffer, imageIndex);

		// Mask pipeline
		_maskPipeline->render(commandBuffer, imageIndex);

		// Outline pipeline
		_outlinePipeline->render(commandBuffer, imageIndex);
	}
	_graphicsPipeline->endRender(commandBuffer);
}

void Application::updateUniformBuffer(uint32_t currentImage)
{
	int samplesPerFrame = 100;
	_totalNumberOfSamples = samplesPerFrame;

	UniformBufferObject ubo;
	ubo.modelView = _modelViewController->getModelView();
	ubo.aperture = 0.02f;
	ubo.focusDistance = 2.0f;
	// TODO near/far are hardcoded (being used in physicsEngine too)
	ubo.projection = glm::perspective(glm::radians(90.0f), _swapChain->getExtent().width / static_cast<float>(_swapChain->getExtent().height), 0.1f, 1000.0f);
	ubo.projection[1][1] *= -1; // Inverting Y for Vulkan, https://matthewwellings.com/blog/the-new-vulkan-coordinate-system/
	ubo.modelViewInverse = glm::inverse(ubo.modelView);
	ubo.projectionInverse = glm::inverse(ubo.projection);
	ubo.totalNumberOfSamples = _totalNumberOfSamples;
	ubo.numberOfSamples = samplesPerFrame;
	ubo.numberOfBounces = 4;
	ubo.randomSeed = 1;
	ubo.gammaCorrection = true;
	ubo.hasSky = false;

	_uniformBuffers[currentImage]->setValue(ubo);
}
