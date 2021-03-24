//--------------------------------------------------
// Atta Parallel
// threadManager.cpp
// Date: 2021-01-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/parallel/threadManager.h>
#include <atta/helpers/log.h>
//#include "simulator/graphics/renderers/rayTracing/rayTracingVulkan/rayTracing.h"

namespace atta
{
	ThreadManager::ThreadManager(PipelineSetup pipelineSetup):
		_shouldFinish(false)
	{
		//----- Calculate quantity of workers -----//
		unsigned maxSystemCores = std::max(1u, std::thread::hardware_concurrency());
		// maxSystemCores-1 because main thread helps out, too
		_qtyWorkersToCreate = pipelineSetup.generalConfig.qtyThreads==-1 ? 
			maxSystemCores-1 : 
			pipelineSetup.generalConfig.qtyThreads;

		Log::info("ThreadManager", "Detected $0 cores, $1 workers will be created", maxSystemCores, _qtyWorkersToCreate);

		//                                                             ,-------------------.
		//                                                             v                   |
		// Barrier to syncronize generalist workers + main thread (start -> physics -> render -> robots -> end)
		_setupStageBarrier = std::make_shared<Barrier>(_qtyWorkersToCreate);
		_physicsStageBarrier = std::make_shared<Barrier>(_qtyWorkersToCreate);
		_renderingStageBarrier = std::make_shared<Barrier>(_qtyWorkersToCreate);
		_robotStageBarrier = std::make_shared<Barrier>(_qtyWorkersToCreate);

		//---------- Core ----------//
		_scene = pipelineSetup.generalConfig.scene;
		_dimensionMode = pipelineSetup.generalConfig.dimensionMode;
		_accelerator = pipelineSetup.physicsStage.accelerator;

		//---------- Physics stage ----------//
		_physicsEngine = pipelineSetup.physicsStage.physicsEngine;

		//---------- Robot stage ----------//
		_robotProcessing = pipelineSetup.robotStage.robotProcessing;
		_runAfterRobots = pipelineSetup.robotStage.runAfterRobots;
		
		//---------- Rendering stage ----------//
		_vkCore = pipelineSetup.renderingStage.vkCore;
		_commandPool = std::make_shared<vk::CommandPool>(_vkCore->getDevice(), vk::CommandPool::DEVICE_QUEUE_FAMILY_GRAPHICS, vk::CommandPool::QUEUE_THREAD_MANAGER, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
		_renderers = pipelineSetup.renderingStage.renderers;

		createGeneralistWorkers();
		createGuiWorker();
		createCoreObjects();
		createPhysicsObjects();
		createRenderingObjects();
	}

	ThreadManager::~ThreadManager()
	{
		Log::info("ThreadManager", "Execution finished, stopping workers...");
		// Wait barriers to finish thread loop (to evaluate _shouldFinish)
		_physicsStageBarrier->wait();
		_robotStageBarrier->wait();

		// Ask threads to stop
		// If you ask to finish before waiting for the physics barriers some threads 
		// may finish before others, resulting in some threads being stuck in the barrier (waiting the finished threads)
		for(auto& w : _workersGen)
			w->setShouldFinish(true);
		_workerGui->setShouldFinish(true);

		// Wait last barrier (doing this ensured that all threads are in this stage)
		_renderingStageBarrier->wait();

		for(auto& w : _workersGen)
			w->setShouldFinish(true);

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

		for(unsigned i = 0; i < _qtyWorkersToCreate-1; i++)
		{
			_workersGen.push_back(std::make_shared<WorkerGeneralist>(info));
			_threads.push_back(std::thread(std::ref(*_workersGen[i])));
		}
		Log::success("ThreadManager", "Created $0 generalist workers.", _qtyWorkersToCreate-1);
	}

	void ThreadManager::createGuiWorker()
	{
		_workerGui = std::make_shared<WorkerGui>(_vkCore,
			_dimensionMode == DIM_MODE_3D?
					WorkerGui::CAMERA_CONTROL_TYPE_3D:
					WorkerGui::CAMERA_CONTROL_TYPE_2D
				);
		_workerGui->setRenderers(_renderers);

		// Create thread from callable workerGui
		_threads.push_back(std::thread(std::ref(*_workerGui)));
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

	void ThreadManager::run()
	{
		//------------------------- Setup -----------------------//
		_setupStageBarrier->wait();

		//-------------------- Pipeline stages ------------------//
		auto lastTime = std::chrono::high_resolution_clock::now();
		auto currTime = std::chrono::high_resolution_clock::now();
		while(!_shouldFinish)
		{
			currTime = std::chrono::high_resolution_clock::now();
			auto start = std::chrono::time_point_cast<std::chrono::microseconds>(lastTime).time_since_epoch().count();
			auto end = std::chrono::time_point_cast<std::chrono::microseconds>(currTime).time_since_epoch().count();
			float dt = (end-start)/1000000.0;
			//dt/=20;
			lastTime = currTime;

			//-------------------- Physics ----------------------//
			if(_physicsEngine != nullptr)
				_physicsEngine->stepPhysics(dt);

			_physicsStageBarrier->wait();
			//--------------------- Robots ----------------------//
			switch(_robotProcessing)
			{
				case ROBOT_PROCESSING_SEQUENTIAL:
					for(auto robot : _scene->getRobots())
						robot->run(dt);
					break;
				case ROBOT_PROCESSING_PARALLEL_CPU:
					// Populate work to the threads
					break;
				case ROBOT_PROCESSING_PARALLEL_GPU:
					break;
			}

			if(_runAfterRobots)
				_runAfterRobots();

			_robotStageBarrier->wait();
			//-------------------- Rendering --------------------//

			_renderingStageBarrier->wait();

			_shouldFinish = true;
		}
	}
}
