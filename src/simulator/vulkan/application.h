//--------------------------------------------------
// Robot Simulator
// application.h
// Date: 2020-06-21
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
#include "pipeline/graphicsPipeline.h"
#include "pipeline/linePipeline.h"
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
#include "modelViewController.h"
#include "ui/userInterface.h"
#include "rayTracing/rayTracing.h"
#include "../scene.h"

class Application
{
	public:
		Application(Scene* scene);
		~Application();

		void run();

		//---------- Camera handling ----------//

		//---------- Callbacks ----------//
		std::function<void(float dt)> onDrawFrame;
		std::function<void(glm::vec3 cameraPos, glm::vec3 raycastRay)> onRaycastClick;
	private:
		void drawFrame();
		void createPipelines();
		void cleanupSwapChain();
		void recreateSwapChain();
		void framebufferResizeCallback() {_framebufferResized = true;}
		void updateUniformBuffer(uint32_t currentImage);
		void createDescriptorPool();
		void render(VkCommandBuffer commandBuffer, int imageIndex);

		// User Interface
		void createUserInterface();

		// Special Scene Objects
		void createSpecialSceneObjects();

		// Window callbacks
		void onKey(int key, int scancode, int action, int mods);
		void onCursorPosition(double xpos, double ypos);
		void onMouseButton(int button, int action, int mods);
		void onScroll(double xoffset, double yoffset);

		Window* _window;
		Instance* _instance;
		DebugMessenger* _debugMessenger;
		PhysicalDevice* _physicalDevice;
		Device* _device;
		Surface* _surface;
		SwapChain* _swapChain;

		RenderPass* _renderPass;
		GraphicsPipeline* _graphicsPipeline;
		LinePipeline* _linePipeline;

		CommandPool* _commandPool;
		CommandBuffers* _commandBuffers;
		StagingBuffer* _stagingBuffer;
		DescriptorSetLayout* _descriptorSetLayout;
		DescriptorPool* _descriptorPool;
		DescriptorSets* _descriptorSets;
		Texture* _texture;
		DepthBuffer* _depthBuffer;
		ColorBuffer* _colorBuffer;

		ModelViewController* _modelViewController;

		UserInterface* _userInterface;
		RayTracing* _rayTracing;

		Scene* _scene;

		std::vector<FrameBuffer*> _frameBuffers;
		std::vector<UniformBuffer*> _uniformBuffers;

		std::vector<Semaphore*> _imageAvailableSemaphores;
		std::vector<Semaphore*> _renderFinishedSemaphores;
		std::vector<Fence*> _inFlightFences;
		std::vector<VkFence> _imagesInFlight;

		size_t _currentFrame;
		bool _framebufferResized;
		double _time;

		// Ray tracing
		bool _enableRayTracing;
		int _totalNumberOfSamples;
		bool _splitRender;

		// Camera parameters
		std::vector<RayTracing*> _camerasRayTracing;
		std::vector<GraphicsPipeline*> _camerasRasterization;
};

#endif// APPLICATION_H
