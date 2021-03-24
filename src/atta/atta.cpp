//--------------------------------------------------
// Atta Robot Simulator
// atta.cpp
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/atta.h>
#include <iostream>
#include <atta/project.h>
#include <atta/helpers/log.h>
#include <atta/graphics/vulkan/vulkanCore.h>
#include <atta/core/accelerator.h>
#include <atta/core/scene.h>
#include <atta/graphics/renderers/rastRenderer/rastRenderer.h>
//#include "simulator/graphics/renderers/renderer2D/renderer2D.h"
//#include "simulator/graphics/renderers/rayTracing/rayTracingVulkan/rayTracing.h"
//#include "simulator/graphics/renderers/rayTracing/rayTracingCPU/rayTracing.h"
#include <atta/physics/physicsEngine.h>

namespace atta
{
	Atta::Atta(CreateInfo info):
		_info(info)
	{
		// Add robot objects to scene
		for(auto robot : _info.robots)
			_info.objects.push_back(robot->getRootObject());

		// Create scene
		Scene::CreateInfo sceneInfo = 
		{
			.objects = _info.objects,
			.robots = _info.robots
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

	Atta Atta::createFromProject(Project project)
	{
		Atta atta = Atta(project.getAttaCreateInfo());
		return atta;
	}

	void Atta::run()
	{
		_threadManager->run();
	}

	ThreadManager::GeneralConfig Atta::populateTMGeneralConfig()
	{
		ThreadManager::GeneralConfig config = {
			.scene = _scene,
			.dimensionMode = _info.dimensionMode
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
		return {
			.robotProcessing = _info.robotProcessing,
			.runAfterRobots = _info.runAfterRobots
		};
	}

	ThreadManager::RenderingStage Atta::populateTMRenderingStage()
	{
		// Create vulkan core
		std::shared_ptr<vk::VulkanCore> vkCore = std::make_shared<vk::VulkanCore>();
		std::shared_ptr<vk::CommandPool> commandPool = std::make_shared<vk::CommandPool>(vkCore->getDevice(), vk::CommandPool::DEVICE_QUEUE_FAMILY_GRAPHICS, vk::CommandPool::QUEUE_THREAD_MANAGER, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
		vkCore->createBuffers(_scene, commandPool);

		ThreadManager::RenderingStage renderingStage;

		//---------- Create renderers from dimension info ----------//
		if(_info.dimensionMode == atta::DIM_MODE_3D)
		{
			// Create rasterization render
			RastRenderer::CreateInfo rastRendInfo = {
				.vkCore = vkCore,
				.commandPool = commandPool,
				.width = 1200,
				.height = 900,
				.scene = _scene,
				.viewMat = atta::lookAt(vec3(-10,10,-10), vec3(0,0,0), vec3(0,1,0)),
				.projMat = atta::perspective(atta::radians(60.0), 1200.0/900, 0.01f, 1000.0f)
			};
			std::shared_ptr<RastRenderer> rast = std::make_shared<RastRenderer>(rastRendInfo);

			//// Create ray tracing vulkan render
			//rt::vk::RayTracing::CreateInfo rtVkRendInfo = 
			//{
			//	.vkCore = vkCore,
			//	.commandPool = commandPool,
			//	.width = 1200,
			//	.height = 900,
			//	.scene = _scene,
			//	.viewMat = atta::lookAt(vec3(0,5,7), vec3(0,0,0), vec3(0,1,0)),
			//	.projMat = atta::perspective(atta::radians(60.0), 1200.0/900, 0.01f, 1000.0f)
			//};
			//std::shared_ptr<rt::vk::RayTracing> rtVk = std::make_shared<rt::vk::RayTracing>(rtVkRendInfo);

			// Create ray tracing CPU render
			//rt::cpu::RayTracing::CreateInfo rtCPURendInfo = {
			//	.vkCore = vkCore,
			//	.commandPool = commandPool,
			//	.width = 1200,
			//	.height = 900,
			//	.scene = _scene,
			//	.viewMat = atta::lookAt(vec3(-10,10,-10), vec3(0,0,0), vec3(0,1,0)),
			//	.projMat = atta::perspective(atta::radians(60.0), 1200.0/900, 0.01f, 1000.0f)
			//};
			//std::shared_ptr<rt::cpu::RayTracing> rtCPU = std::make_shared<rt::cpu::RayTracing>(rtCPURendInfo);

			// Populate return
			renderingStage = {
				.vkCore = vkCore,
				.renderers = {
					//rtVk,
					rast
				}
			};
		}
		else if(_info.dimensionMode == atta::DIM_MODE_2D)
		{
			//Renderer2D::CreateInfo rend2DInfo = {
			//	.vkCore = vkCore,
			//	.commandPool = commandPool,
			//	.width = 1200,
			//	.height = 900,
			//	.scene = _scene
			//};
			//std::shared_ptr<Renderer2D> renderer2D = std::make_shared<Renderer2D>(rend2DInfo);

			renderingStage = {
				.vkCore = vkCore,
				//.renderers = {
				//	renderer2D
				//}
			};
		}

		return renderingStage;
	}
}
