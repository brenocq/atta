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

namespace atta
{
	Atta::Atta(CreateInfo createInfo)
	{
		//---------- Core ----------//
		// Create scene
		Scene::CreateInfo sceneInfo = 
		{
			.objects = createInfo.objects
		};
		std::shared_ptr<Scene> scene = std::make_shared<Scene>(sceneInfo);

		// Create accelerator
		Accelerator::CreateInfo accInfo = 
		{
			.objects = createInfo.objects
		};
		std::shared_ptr<Accelerator> accelerator = std::make_shared<Accelerator>(accInfo);

		//---------- Physics stage ----------//

		//---------- Rendering stage ----------//
		// Create vulkan core
		auto vulkanCore = std::make_shared<vk::VulkanCore>();
		vulkanCore->createBuffers(scene);

		// Create main render
		RastRenderer::CreateInfo rastRendInfo {
			.vkCore = vulkanCore,
			.width = 1200,
			.height = 900,
			.scene = scene,
			.viewMat = atta::lookAt(vec3(3,3,3), vec3(0,0,0), vec3(0,1,0)),
			.projMat = atta::perspective(atta::radians(90.0), 1200.0/900, 0.01f, 1000.0f)
		};
		std::shared_ptr<RastRenderer> rast = std::make_shared<RastRenderer>(rastRendInfo);

		//---------- Create thread manager ----------//
		ThreadManager::PipelineSetup pipelineSetup = {
			.generalConfig = {
				.scene = scene
			},
			.physicsStage = {
				.accelerator = accelerator
			},
			.renderingStage = {
				.vkCore = vulkanCore,
				.mainRenderer = rast
			},
			.robotStage = {}
		};

		// Create thread manager
		_threadManager = std::make_shared<ThreadManager>(pipelineSetup);
	}

	Atta::~Atta()
	{
	}

	void Atta::run()
	{
		_threadManager->run();
	}
}
