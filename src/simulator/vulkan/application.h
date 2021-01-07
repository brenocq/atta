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
#include <memory>

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
		std::shared_ptr<Window> _window;
		std::shared_ptr<Instance> _instance;
		std::unique_ptr<DebugMessenger> _debugMessenger;
		std::shared_ptr<Surface> _surface;
		std::shared_ptr<PhysicalDevice> _physicalDevice;
		std::shared_ptr<Device> _device;
		std::shared_ptr<SwapChain> _swapChain;

		// Command Buffers
		std::shared_ptr<CommandPool> _commandPool;
		std::unique_ptr<CommandBuffers> _commandBuffers;

		// Pipelines
		std::unique_ptr<GraphicsPipeline> _graphicsPipeline;
		std::unique_ptr<LinePipeline> _linePipeline;
		std::unique_ptr<MaskPipeline> _maskPipeline;
		std::unique_ptr<OutlinePipeline> _outlinePipeline;
		std::unique_ptr<SkyboxPipeline> _skyboxPipeline;

		// Other renderers
		std::unique_ptr<UserInterface> _userInterface;
		RayTracing* _rayTracing;
		atta::rt::RayTracingCPU* _rayTracingCPU;

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
