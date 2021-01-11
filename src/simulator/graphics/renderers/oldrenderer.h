//--------------------------------------------------
// Robot Simulator
// application.h
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RENDERER_H
#define ATTA_RENDERER_H

#include <memory>
#include <vector>

#include "simulator/scene.h"
#include "simulator/graphics/core/window.h"
#include "simulator/graphics/vulkan/vulkan.h"
#include "rayTracing/rayTracingVulkan/rayTracing.h"
#include "rayTracing/rayTracingCPU/rayTracingCPU.h"
#include "gui/userInterface.h"

namespace atta
{
	class Renderer
	{
		public:
			enum RenderingType {
				RASTERIZATION,
				RAY_TRACING_VULKAN,
				RAY_TRACING_ATTA_CPU,
				RAY_TRACING_ATTA_CUDA,
			};

			Renderer(std::shared_ptr<Window> window, std::shared_ptr<Scene> scene, RenderingType renderingType = RASTERIZATION);
			~Renderer();

			void run();

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
			//void onKey(int key, int scancode, int action, int mods);
			//void onCursorPosition(double xpos, double ypos);
			//void onMouseButton(int button, int action, int mods);
			//void onScroll(double xoffset, double yoffset);

			RenderingType _renderingType;
			// Core clases
			std::shared_ptr<Window> _window;
			std::shared_ptr<vk::Instance> _instance;
			std::unique_ptr<vk::DebugMessenger> _debugMessenger;
			std::shared_ptr<vk::Surface> _surface;
			std::shared_ptr<vk::PhysicalDevice> _physicalDevice;
			std::shared_ptr<vk::Device> _device;
			std::shared_ptr<vk::SwapChain> _swapChain;

			// Command Buffers
			std::shared_ptr<vk::CommandPool> _commandPool;
			std::unique_ptr<vk::CommandBuffers> _commandBuffers;

			// Pipelines
			std::unique_ptr<vk::GraphicsPipeline> _graphicsPipeline;
			std::unique_ptr<vk::LinePipeline> _linePipeline;
			std::unique_ptr<vk::MaskPipeline> _maskPipeline;
			std::unique_ptr<vk::OutlinePipeline> _outlinePipeline;
			std::unique_ptr<vk::SkyboxPipeline> _skyboxPipeline;

			// Other renderers
			std::unique_ptr<UserInterface> _userInterface;
			std::shared_ptr<rt::RayTracing> _rayTracing;
			std::shared_ptr<rt::RayTracingCPU> _rayTracingCPU;

			// modelViewController
			std::shared_ptr<ModelViewController> _modelViewController;
			std::vector<std::shared_ptr<UniformBuffer>> _uniformBuffers;

			// Scene info
			std::shared_ptr<Scene> _scene;

			// Image/swapchain synchronization
			std::vector<std::shared_ptr<vk::Semaphore>> _imageAvailableSemaphores;
			std::vector<std::shared_ptr<vk::Semaphore>> _renderFinishedSemaphores;
			std::vector<std::shared_ptr<vk::Fence>> _inFlightFences;
			std::vector<VkFence> _imagesInFlight;

			size_t _currentFrame;
			bool _framebufferResized;
			double _time;

			// Ray tracing
			bool _enableRayTracing;
			int _totalNumberOfSamples;
			bool _splitRender;

			// Camera sensor parameters
			std::vector<std::shared_ptr<vk::RayTracing>> _camerasRayTracing;
			std::vector<std::shared_ptr<vk::Image>> _camerasRasterizationImage;
			std::vector<std::shared_ptr<vk::ImageView>> _camerasRasterizationImageView;
			std::vector<std::shared_ptr<vk::GraphicsPipeline>> _camerasRasterization;
			std::vector<std::shared_ptr<vk::UniformBuffer>> _camerasUniformBuffer;
	};
}
#endif// ATTA_RENDERER_H
