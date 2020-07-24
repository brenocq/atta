//--------------------------------------------------
// Robot Simulator
// application.h
// Date: 21/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef APPLICATION_H
#define APPLICATION_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

#include <vector>

#include "window.h"
#include "instance.h"
#include "debugMessenger.h"
#include "physicalDevice.h"
#include "device.h"
#include "surface.h"
#include "swapChain.h"
#include "graphicsPipeline.h"
#include "frameBuffer.h"
#include "commandPool.h"
#include "commandBuffers.h"
#include "semaphore.h"
#include "fence.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "stagingBuffer.h"
#include "descriptorSetLayout.h"
#include "descriptorPool.h"
#include "descriptorSets.h"
#include "uniformBuffer.h"
#include "texture.h"
#include "depthBuffer.h"
#include "colorBuffer.h"
#include "model.h"
#include "ui/userInterface.h"
#include "rayTracing/rayTracing.h"
#include "../scene.h"

class Application
{
	public:
	Application();
	~Application();

	void run();

	private:
	void drawFrame();
	void createCommandBuffers();
	void cleanupSwapChain();
	void recreateSwapChain();
	void framebufferResizeCallback() {_framebufferResized = true;}
	void createBuffers();
	void updateUniformBuffer(uint32_t currentImage);
	void createDescriptorPool();
	void render(int i);

	Window* _window;
	Instance* _instance;
	DebugMessenger* _debugMessenger;
	PhysicalDevice* _physicalDevice;
	Device* _device;
	Surface* _surface;
	SwapChain* _swapChain;
	GraphicsPipeline* _graphicsPipeline;
	CommandPool* _commandPool;
	CommandBuffers* _commandBuffers;
	//VertexBuffer* _vertexBuffer;
	//IndexBuffer* _indexBuffer;
	StagingBuffer* _stagingBuffer;
	DescriptorSetLayout* _descriptorSetLayout;
	DescriptorPool* _descriptorPool;
	DescriptorSets* _descriptorSets;
	Texture* _texture;
	DepthBuffer* _depthBuffer;
	ColorBuffer* _colorBuffer;

	UserInterface* _userInterface;
	RayTracing* _rayTracing;

	Scene* _scene;
	Model* _model;

	std::vector<FrameBuffer*> _frameBuffers;
	std::vector<VkCommandBuffer> _commandBuffersTest;
	std::vector<UniformBuffer*> _uniformBuffers;

	std::vector<Semaphore*> _imageAvailableSemaphores;
	std::vector<Semaphore*> _renderFinishedSemaphores;
	std::vector<Fence*> _inFlightFences;
	std::vector<VkFence> _imagesInFlight;

	size_t _currentFrame;
	bool _framebufferResized;
};

#endif// APPLICATION_H
