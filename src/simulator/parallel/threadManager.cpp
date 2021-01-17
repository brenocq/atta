//--------------------------------------------------
// Atta Parallel
// threadManager.cpp
// Date: 2021-01-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "threadManager.h"
#include "simulator/helpers/log.h"

namespace atta
{
	ThreadManager::ThreadManager(PipelineSetup pipelineSetup)
	{
		//----- Calculate quantity of workers -----//
		unsigned maxSystemCores = std::max(1u, std::thread::hardware_concurrency());
		// maxSystemCores-1 because main thread helps out, too
		_qtyWorkersToCreate = pipelineSetup.generalConfig.qtyThreads==-1 ? maxSystemCores-1 : qtyThreads;

		Log::info("ThreadManager", "Detected $0 cores, $1 workers can be created", maxSystemCores, _qtyWorkersToCreate);

		//                                                             ,-------------------.
		//                                                             v                   |
		// Barrier to syncronize generalist workers state (start -> physics -> render -> robots -> end)
		_generalistStateBarrier = std::make_shared<Barrier>(_qtyWorkersToCreate-1);

		createGeneralistWorkers();
		createGuiWorker(pipelineSetup.renderingStage.vkCore);
	}

	ThreadManager::~ThreadManager()
	{
		for(auto& t : _threads)
			t.join();
	}


	void ThreadManager::createGeneralistWorkers()
	{
		// Create generalists
		for(unsigned i = 0; i < _qtyWorkersToCreate-1; ++i)
		{
			_workersGen.push_back(std::make_shared<WorkerGeneralist>(_generalistStateBarrier));
			_threads.push_back(std::thread(*_workersGen[i]));
		}
		Log::success("ThreadManager", "Created $0 generalist workers.", _qtyWorkersToCreate-1);
	}

	void ThreadManager::createGuiWorker(std::shared_ptr<vk::VulkanCore> vkCore)
	{
		_workerGui = std::make_shared<WorkerGui>(vkCore);
		_threads.push_back(std::thread(*_workerGui));
		Log::success("ThreadManager", "Created 1 GUI worker.");
	}

	void ThreadManager::setupAttaPipeline(AttaPipelineSetup pipelineSetup)
	{

	}

	void ThreadManager::run()
	{
		// Manage threadManager workers
		while(true)
		{
			//--------------------- Physics ---------------------//
			
			//-------------------- Rendering --------------------//
			//for(auto& render : _renderers)
			//{
			//	//render->render();
			//}

			//--------------------- Robots ----------------------//
		}
	}
}
