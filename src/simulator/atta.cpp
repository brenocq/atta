//--------------------------------------------------
// Atta Robot Simulator
// atta.cpp
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "atta.h"

#include "simulator/helpers/log.h"
#include "simulator/graphics/vulkan/vulkanCore.h"
#include "simulator/core/accelerator.h"
#include "simulator/core/scene.h"
#include "simulator/graphics/renderers/rastRenderer/rastRenderer.h"
#include "simulator/graphics/renderers/renderer2D/renderer2D.h"
#include "simulator/graphics/renderers/rayTracing/rayTracingVulkan/rayTracing.h"
#include "simulator/graphics/renderers/rayTracing/rayTracingCPU/rayTracing.h"
#include "simulator/physics/physicsEngine.h"

namespace atta
{
	Atta::Atta(CreateInfo info):
		_info(info)
	{
		// Create scene
		Scene::CreateInfo sceneInfo = 
		{
			.objects = _info.objects
		};
		_scene = std::make_shared<Scene>(sceneInfo);

		//---------- Create thread manager ----------//
		ThreadManager::PipelineSetup pipelineSetup = {
			.generalConfig = populateTMGeneralConfig(),
			.physicsStage = populateTMPhysicsStage(),
			.robotStage = populateTMRobotStage(),
			.renderingStage = populateTMRenderingStage()
		};
		_threadManager = std::make_shared<ThreadManager>(pipelineSetup);
	}

	Atta::~Atta()
	{
	}

	void Atta::run()
	{
		_threadManager->run();
	}

	ThreadManager::GeneralConfig Atta::populateTMGeneralConfig()
	{
		// Populate return
		ThreadManager::GeneralConfig config = {
			.scene = _scene,
			.dimensionMode = _info.dimensionMode == DIM_MODE_3D ? ThreadManager::DIM_MODE_3D : ThreadManager::DIM_MODE_2D  
		};

		return config;
	}

	ThreadManager::PhysicsStage Atta::populateTMPhysicsStage()
	{
		ThreadManager::PhysicsStage physicsStage;

		if(_info.physicsMode != PHY_MODE_DISABLED)
		{
			// Create accelerator
			Accelerator::CreateInfo accInfo = {
				.objects = _info.objects
			};
			std::shared_ptr<Accelerator> accelerator = std::make_shared<Accelerator>(accInfo);

			// Create physics engine
			phy::PhysicsEngine::CreateInfo phyEngInfo = {
				.accelerator = accelerator
			};
			std::shared_ptr<phy::PhysicsEngine> physicsEngine = std::make_shared<phy::PhysicsEngine>(phyEngInfo);

			// Populate return
			physicsStage = {
				.accelerator = accelerator,
				.physicsEngine = physicsEngine
			};
		}

		return physicsStage;
	}

	ThreadManager::RobotStage Atta::populateTMRobotStage()
	{
		return {};
	}

	ThreadManager::RenderingStage Atta::populateTMRenderingStage()
	{
		// Create vulkan core
		std::shared_ptr<vk::VulkanCore> vkCore = std::make_shared<vk::VulkanCore>();
		vkCore->createBuffers(_scene);

		ThreadManager::RenderingStage renderingStage;

		//---------- Create renderers from dimension info ----------//
		if(_info.dimensionMode == DIM_MODE_3D)
		{
			// Create rasterization render
			RastRenderer::CreateInfo rastRendInfo = {
				.vkCore = vkCore,
				.width = 1200,
				.height = 900,
				.scene = _scene,
				.viewMat = atta::lookAt(vec3(-5,1,-5), vec3(0,0,0), vec3(0,1,0)),
				.projMat = atta::perspective(atta::radians(60.0), 1200.0/900, 0.01f, 1000.0f)
			};
			std::shared_ptr<RastRenderer> rast = std::make_shared<RastRenderer>(rastRendInfo);

			// Create ray tracing vulkan render
			rt::vk::RayTracing::CreateInfo rtVkRendInfo = 
			{
				.vkCore = vkCore,
				.width = 1200,
				.height = 900,
				.scene = _scene,
				.viewMat = atta::lookAt(vec3(-5,1,-5), vec3(0,0,0), vec3(0,1,0)),
				.projMat = atta::perspective(atta::radians(60.0), 1200.0/900, 0.01f, 1000.0f)
			};
			std::shared_ptr<rt::vk::RayTracing> rtVk = std::make_shared<rt::vk::RayTracing>(rtVkRendInfo);

			// Create ray tracing CPU render
			rt::cpu::RayTracing::CreateInfo rtCPURendInfo = {
				.vkCore = vkCore,
				.width = 1200,
				.height = 900,
				.scene = _scene,
				.viewMat = atta::lookAt(vec3(-5,1,-5), vec3(0,0,0), vec3(0,1,0)),
				.projMat = atta::perspective(atta::radians(60.0), 1200.0/900, 0.01f, 1000.0f)
			};
			std::shared_ptr<rt::cpu::RayTracing> rtCPU = std::make_shared<rt::cpu::RayTracing>(rtCPURendInfo);

			// Populate return
			renderingStage = {
				.vkCore = vkCore,
				.renderers = {
					rtVk,
					rast
				}
			};
		}
		else if(_info.dimensionMode == DIM_MODE_2D)
		{
			Renderer2D::CreateInfo rend2DInfo = {
				.vkCore = vkCore,
				.width = 1200,
				.height = 900,
				.scene = _scene
			};
			std::shared_ptr<Renderer2D> renderer2D = std::make_shared<Renderer2D>(rend2DInfo);

			renderingStage = {
				.vkCore = vkCore,
				.renderers = {
					renderer2D
				}
			};
		}

		return renderingStage;
	}
}
