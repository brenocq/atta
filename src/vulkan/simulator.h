#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <string>
#include <vector>
#include "glm.h"
#include "window.h"
#include "instance.h"
#include "debugUtilsMessenger.h"
#include "surface.h"
#include "physicalDevice.h"
#include "device.h"
#include "commandPool.h"
#include "swapChain.h"
#include "depthBuffer.h"
#include "graphicsPipeline.h"
#include "commandBuffers.h"
#include "frameBuffer.h"
#include "semaphore.h"
#include "fence.h"
#include "camera.h"
#include "../assets/scene.h"
#include "../assets/uniformBuffer.h"
#include "../assets/model.h"
#include "../assets/texture.h"

class Simulator
{
	public:
	Simulator();
	~Simulator();

	void initWindow();
	void initInstance();
	void initSurface();
	void initDevice();
	void initSwapChain();
	void run();
	void setScene(Scene* scene) { _scene=scene; }

	bool _isWireFrame;

	private:
	void drawFrame();
	void render(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	UniformBufferObject getUniformBufferObject(VkExtent2D extent);
	void recreateSwapChain();
	void checkFramebufferSize() const;

	Window* _window;
	Instance* _instance;
	Surface* _surface;
	PhysicalDevice *_physicalDevice;
	Device* _device;
	CommandPool* _commandPool;
	SwapChain* _swapChain;
	DepthBuffer* _depthBuffer;
	GraphicsPipeline* _graphicsPipeline;
	CommandBuffers* _commandBuffers;
	Scene* _scene;
	Camera* _camera;
	DebugUtilsMessenger* _debugUtilsMessenger;
	std::vector<FrameBuffer> _swapChainFramebuffers;
	std::vector<UniformBuffer> _uniformBuffers;
	std::vector<Semaphore> _imageAvailableSemaphores;
	std::vector<Semaphore> _renderFinishedSemaphores;
	std::vector<Fence> _inFlightFences;

	size_t _currentFrame;
	uint32_t _totalNumberOfSamples;
};

#endif// SIMULATOR_H
