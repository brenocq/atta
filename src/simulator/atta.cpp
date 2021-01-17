//--------------------------------------------------
// Atta Robot Simulator
// atta.cpp
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "atta.h"

#include "simulator/helpers/log.h"
//#include "simulator/helpers/evaluator.h"

namespace atta
{
	Atta::Atta(CreateInfo createInfo)
	{
		// Create scene
		Scene::CreateInfo sceneInfo = 
		{
			.objects = createInfo.objects
		};
		_scene = std::make_shared<Scene>(sceneInfo);

		// Create accelerator
		Accelerator::CreateInfo accInfo = 
		{
			.objects = createInfo.objects
		};
		_accelerator = std::make_shared<Accelerator>(accInfo);

		// Create vulkan core
		_vulkanCore = std::make_shared<vk::VulkanCore>();
		_vulkanCore->createBuffers(_scene);

		// Create renderers
		RastRenderer::CreateInfo rastRendInfo {
			.vkCore = _vulkanCore,
			.width = 500,
			.height = 500
		};
		std::shared_ptr<RastRenderer> rast = std::make_shared<RastRenderer>(rastRendInfo);
		_renderers.push_back(rast);

		// Initialize physics
		
		//---------- Create thread manager ----------//
		ThreadManager::PipelineSetup pipelineSetup = {
			.generalConfig = {},
			.physicsStage = {},
			.renderingStage = {
				.vkCore = _vulkanCore,
				.renderers = renderers
			},
			.robotStage = {}
		};

		// Create thread pool
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
