//--------------------------------------------------
// Atta Parallel
// threadManager.cpp
// Date: 2021-01-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/parallel/threadManager.h>
#include <atta/helpers/log.h>
#include <atta/helpers/drawer.h>
#include <atta/graphics/renderers/rastRenderer/rastRenderer.h>
#include <atta/graphics/renderers/rayTracing/rayTracingVulkan/rayTracing.h>
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

		Log::verbose("ThreadManager", "Detected $0 cores, $1 workers will be created", maxSystemCores, _qtyWorkersToCreate);
		//                                                             ,-------------------.
		//                                                             v                   |
		// Barrier to syncronize generalist workers + main thread (start -> physics -> render -> robots -> end)
		_setupStageBarrier = std::make_shared<Barrier>(_qtyWorkersToCreate);
		_physicsStageBarrier = std::make_shared<Barrier>(_qtyWorkersToCreate);
		_sensorStageBarrier = std::make_shared<Barrier>(_qtyWorkersToCreate);
		_robotStageBarrier = std::make_shared<Barrier>(_qtyWorkersToCreate);

		//---------- Core ----------//
		_scene = pipelineSetup.generalConfig.scene;
		_dimensionMode = pipelineSetup.generalConfig.dimensionMode;
		_vkCore = pipelineSetup.generalConfig.vkCore;

		//---------- Physics stage ----------//
		_physicsEngine = pipelineSetup.physicsStage.physicsEngine;
		_accelerator = pipelineSetup.physicsStage.accelerator;

		//---------- Sensor stage ----------//
		_commandPool = std::make_shared<vk::CommandPool>(_vkCore->getDevice(), vk::CommandPool::DEVICE_QUEUE_FAMILY_GRAPHICS, vk::CommandPool::QUEUE_THREAD_MANAGER, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
		// TODO Use multiple command buffers to render in parallel?
		_commandBuffers = std::make_shared<vk::CommandBuffers>(_vkCore->getDevice(), _commandPool, 1);

		//---------- Robot stage ----------//
		_robotProcessing = pipelineSetup.robotStage.robotProcessing;
		_runAfterRobots = pipelineSetup.robotStage.runAfterRobots;

		//---------- UI config ----------//
		_guiRenderer = pipelineSetup.uiConfig.guiRenderer;
		_runBeforeWorkerGuiRender = pipelineSetup.uiConfig.runBeforeWorkerGuiRender;
		_handleKeyboard = pipelineSetup.uiConfig.handleKeyboard;
		
		//---------- Create objects ----------//
		createGeneralistWorkers();
		createGuiWorker();
		createCoreObjects();
		createPhysicsObjects();
		createRenderingObjects();
	}

	ThreadManager::~ThreadManager()
	{
		Log::verbose("ThreadManager", "Execution finished, stopping workers...");
		// Wait barriers to finish thread loop (to evaluate _shouldFinish)
		_physicsStageBarrier->wait();
		_sensorStageBarrier->wait();

		// Ask threads to stop
		// If you ask to finish before waiting for the physics barriers some threads 
		// may finish before others, resulting in some threads being stuck in the barrier (waiting the finished threads)
		for(auto& w : _workersGen)
			w->setShouldFinish(true);
		_workerGui->setShouldFinish(true);

		// Wait last barrier (doing this ensured that all threads are in this stage)
		_robotStageBarrier->wait();

		for(auto& w : _workersGen)
			w->setShouldFinish(true);

		for(size_t i=0;i<_threads.size();i++)
			_threads[i].join();
	}


	void ThreadManager::createGeneralistWorkers()
	{
		WorkerGeneralist::CreateInfo info =
		{
			.setupStageBarrier = _setupStageBarrier,
			.physicsStageBarrier = _physicsStageBarrier,
			.sensorStageBarrier = _sensorStageBarrier,
			.robotStageBarrier = _robotStageBarrier
		};

		for(unsigned i = 0; i < _qtyWorkersToCreate-1; i++)
		{
			_workersGen.push_back(std::make_shared<WorkerGeneralist>(info));
			_threads.push_back(std::thread(std::ref(*_workersGen[i])));
		}
		Log::verbose("ThreadManager", "Created $0 generalist workers.", _qtyWorkersToCreate-1);
	}

	void ThreadManager::createGuiWorker()
	{
		_workerGui = std::make_shared<WorkerGui>(_vkCore, _scene,
			_dimensionMode == DIM_MODE_3D?
					WorkerGui::CAMERA_CONTROL_TYPE_3D:
					WorkerGui::CAMERA_CONTROL_TYPE_2D,
				_guiRenderer,
				_runBeforeWorkerGuiRender,
				_handleKeyboard
				);

		// Create thread from callable workerGui
		_threads.push_back(std::thread(std::ref(*_workerGui)));
		Log::verbose("ThreadManager", "Created GUI worker.");
	}

	void ThreadManager::createCoreObjects()
	{

	}

	void ThreadManager::createPhysicsObjects()
	{

	}

	void ThreadManager::createRenderingObjects()
	{
		for(auto& object : _scene->getObjectsFlat())
		{
			if(object->getType() == "Camera")
			{
				std::shared_ptr<Camera> camera = std::static_pointer_cast<Camera>(object);

				// Create rasterization render
				RastRenderer::CreateInfo rastRendInfo = {
					.vkCore = _vkCore,
					.commandPool = _commandPool,
					.width = camera->getWidth(),
					.height =  camera->getHeight(),
					.scene = _scene,
					.viewMat = atta::lookAt(vec3(-10,10,-10), vec3(0,0,0), vec3(0,1,0)),
					.projMat = atta::perspective(atta::radians(camera->getFov()), camera->getWidth()/camera->getHeight(), 0.01f, 1000.0f)
				};
				std::shared_ptr<RastRenderer> rast = std::make_shared<RastRenderer>(rastRendInfo);
				_cameraRenderers.push_back(std::static_pointer_cast<Renderer>(rast));
				_cameras.push_back(camera);
			}
		}
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
			//-------------------- Sensor --------------------//
			{
				// Update camera renderers view matrix
				for(int i=0;i<_cameraRenderers.size();i++)
					_cameraRenderers[i]->updateCameraMatrix(atta::inverse(_cameras[i]->getModelMat()));

				// Render images
				VkCommandBuffer commandBuffer = _commandPool->beginSingleTimeCommands();
				{
					for(int i=0;i<_cameraRenderers.size();i++)
						_cameraRenderers[i]->render(commandBuffer);
				}
				_commandPool->endSingleTimeCommands(commandBuffer);
				// Copy image to buffer
				for(int i=0;i<_cameras.size();i++)
				{
					auto buffer = _cameraRenderers[i]->getImage()->getBuffer(_commandPool);
					_cameras[i]->setBuffer(buffer);
				}
			}

			_sensorStageBarrier->wait();
			//--------------------- Robots ----------------------//
			Drawer::updateBufferMemory(_vkCore, _commandPool);// Send drawer data to GPU
			Drawer::clear();// Clear drawer data to receive new lines/points
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

			if(_workerGui->getShouldFinish())
				_shouldFinish = true;
		}
	}
}
