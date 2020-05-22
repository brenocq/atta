#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <string>
#include <vector>
#include "window.h"
#include "instance.h"
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

	bool _isWireFrame;
	private:
	void drawFrame();
	void render(VkCommandBuffer commandBuffer, uint32_t imageIndex);

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
	std::vector<FrameBuffer> _swapChainFramebuffers;
	std::vector<Semaphore> _imageAvailableSemaphores;
	std::vector<Semaphore> _renderFinishedSemaphores;
	std::vector<Fence> _inFlightFences;

	size_t _currentFrame;
};

#endif// SIMULATOR_H
