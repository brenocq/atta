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
#include "pipeline/outlinePipeline.h"
#include "pipeline/maskPipeline.h"
#include "pipeline/skyboxPipeline.h"
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
#include "gui/userInterface.h"
#include "rayTracing/rayTracing.h"
#include "rayTracing/rayTracingCPU/rayTracingCPU.h"
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
		void render(VkCommandBuffer commandBuffer, int imageIndex);

		// User Interface
		void createUserInterface();

		// Special Scene Objects
		void createSpecialSceneObjects();
		void updateSpecialSceneObjects();

		// Window callbacks
		void onKey(int key, int scancode, int action, int mods);
		void onCursorPosition(double xpos, double ypos);
		void onMouseButton(int button, int action, int mods);
		void onScroll(double xoffset, double yoffset);

		// Core clases
		Window* _window;
		Instance* _instance;
		DebugMessenger* _debugMessenger;
		PhysicalDevice* _physicalDevice;
		Device* _device;
		Surface* _surface;
		SwapChain* _swapChain;

		// Pipelines
		GraphicsPipeline* _graphicsPipeline;
		LinePipeline* _linePipeline;
		MaskPipeline* _maskPipeline;
		OutlinePipeline* _outlinePipeline;
		SkyboxPipeline* _skyboxPipeline;

		// Other renderers
		UserInterface* _userInterface;
		RayTracing* _rayTracing;
		atta::rt::RayTracingCPU* _rayTracingCPU;

		// Command Buffers
		CommandPool* _commandPool;
		CommandBuffers* _commandBuffers;

		// modelViewController
		ModelViewController* _modelViewController;
		std::vector<UniformBuffer*> _uniformBuffers;

		// Scene info
		Scene* _scene;

		// Image/swapchain synchronization
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

		// Camera sensor parameters
		std::vector<RayTracing*> _camerasRayTracing;
		std::vector<Image*> _camerasRasterizationImage;
		std::vector<ImageView*> _camerasRasterizationImageView;
		std::vector<GraphicsPipeline*> _camerasRasterization;
		std::vector<UniformBuffer*> _camerasUniformBuffer;
};

#endif// APPLICATION_H
