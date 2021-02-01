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
		_qtyWorkersToCreate = pipelineSetup.generalConfig.qtyThreads==-1 ? 
			maxSystemCores-1 : 
			pipelineSetup.generalConfig.qtyThreads;

		Log::info("ThreadManager", "Detected $0 cores, $1 workers can be created", maxSystemCores, _qtyWorkersToCreate);

		//                                                             ,-------------------.
		//                                                             v                   |
		// Barrier to syncronize generalist workers + main thread (start -> physics -> render -> robots -> end)
		_setupStageBarrier = std::make_shared<Barrier>(_qtyWorkersToCreate);
		_physicsStageBarrier = std::make_shared<Barrier>(_qtyWorkersToCreate);
		_renderingStageBarrier = std::make_shared<Barrier>(_qtyWorkersToCreate);
		_robotStageBarrier = std::make_shared<Barrier>(_qtyWorkersToCreate);


		//---------- Core ----------//
		_scene = pipelineSetup.generalConfig.scene;
		_accelerator = pipelineSetup.physicsStage.accelerator;

		//---------- Physics stage ----------//
		_physicsEngine = pipelineSetup.physicsStage.physicsEngine;
		
		//---------- Rendering stage ----------//
		_vkCore = pipelineSetup.renderingStage.vkCore;
		_renderers.push_back(pipelineSetup.renderingStage.mainRenderer);

		createGeneralistWorkers();
		createGuiWorker();
		createCoreObjects();
		createPhysicsObjects();
		createRenderingObjects();

		//---------- Robot stage ----------//

		populateThreadsWork();
	}

	ThreadManager::~ThreadManager()
	{
		for(auto& t : _threads)
			t.join();
	}


	void ThreadManager::createGeneralistWorkers()
	{
		WorkerGeneralist::CreateInfo info =
		{
			.setupStageBarrier = _setupStageBarrier,
			.physicsStageBarrier = _physicsStageBarrier,
			.renderingStageBarrier = _renderingStageBarrier,
			.robotStageBarrier = _robotStageBarrier
		};

		for(unsigned i = 0; i < _qtyWorkersToCreate-1; ++i)
		{
			_workersGen.push_back(std::make_shared<WorkerGeneralist>(info));
			_threads.push_back(std::thread(*_workersGen[i]));
		}
		Log::success("ThreadManager", "Created $0 generalist workers.", _qtyWorkersToCreate-1);
	}

	void ThreadManager::createGuiWorker()
	{
		_workerGui = std::make_shared<WorkerGui>(_vkCore);

		// Create commands vector and images to copy vector
		std::vector<std::function<void(VkCommandBuffer commandBuffer)>> commands;
		std::vector<WorkerGui::ImageCopy> imageCopies;

		for(auto renderer : _renderers)
		{
			commands.push_back([renderer](VkCommandBuffer commandBuffer){ renderer->render(commandBuffer); });

			imageCopies.push_back((WorkerGui::ImageCopy){
					.image = renderer->getImage(),
					.extent = renderer->getImage()->getExtent(),
					.offset = {0,0}
					});
		}

		_workerGui->setCommands(commands);
		_workerGui->setImageCopies(imageCopies);
		_workerGui->setMainRenderer(_renderers[0]);

		// Create thread from callable workerGui
		_threads.push_back(std::thread(*_workerGui));
		Log::success("ThreadManager", "Created 1 GUI worker.");
	}

	void ThreadManager::createCoreObjects()
	{

	}

	void ThreadManager::createPhysicsObjects()
	{

	}

	void ThreadManager::createRenderingObjects()
	{

	}

	void ThreadManager::populateThreadsWork()
	{
	}

	void ThreadManager::run()
	{
		//------------------------- Setup -----------------------//
		_setupStageBarrier->wait();
	
		//-------------------- Pipeline stages ------------------//
		auto lastTime = std::chrono::high_resolution_clock::now();
		auto currTime = std::chrono::high_resolution_clock::now();
		while(true)
		{
			currTime = std::chrono::high_resolution_clock::now();
			auto start = std::chrono::time_point_cast<std::chrono::microseconds>(lastTime).time_since_epoch().count();
			auto end = std::chrono::time_point_cast<std::chrono::microseconds>(currTime).time_since_epoch().count();
			float dt = (end-start)/1000000.0;
			//dt/=10;
			lastTime = currTime;

			//-------------------- Physics ----------------------//
			_physicsEngine->stepPhysics(dt);
			
			_physicsStageBarrier->wait();
			//-------------------- Rendering --------------------//

			_renderingStageBarrier->wait();
			//--------------------- Robots ----------------------//
			
			_robotStageBarrier->wait();
		}
	}
}
