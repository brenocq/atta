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
		// Create thread pool
		_threadPool = std::make_shared<ThreadPool>();
		_threadPool->createGeneralistWorkers();
		_threadPool->createGuiWorker();

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

		// Create renderers
		std::shared_ptr<RastRenderer> rast = std::make_shared<RastRenderer>();
		_renderers.push_back(rast);

		// Initialize physics
		
		// Initialize renderer
		//_renderer = std::make_shared<Renderer>(_window, _scene);

		// Initialize objects

		// Start simulator
	}

	Atta::~Atta()
	{
	}

	void Atta::run()
	{
		// Manage threadPool workers
		while(true)
		{

		}
	}
}
