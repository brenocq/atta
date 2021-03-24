//--------------------------------------------------
// Atta Parallel
// threadManager.h
// Date: 2021-01-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PARALLEL_THREAD_MANAGER_H
#define ATTA_PARALLEL_THREAD_MANAGER_H

#include <vector>
#include <thread>
#include <memory>
#include <functional>
#include <atta/parallel/barrier.h>
#include <atta/parallel/workerGeneralist.h>
#include <atta/parallel/workerGui.h>
#include <atta/core/common.h>
#include <atta/core/scene.h>
#include <atta/core/accelerator.h>
#include <atta/graphics/renderers/renderer.h>
#include <atta/graphics/vulkan/vulkanCore.h>
#include <atta/physics/physicsEngine.h>

namespace atta
{
	class ThreadManager
	{
		public:
			struct GeneralConfig {
				int qtyThreads = -1;
				std::shared_ptr<Scene> scene;
				DimMode dimensionMode = DIM_MODE_3D;
			};

			struct PhysicsStage {
				std::shared_ptr<Accelerator> accelerator = nullptr;
				std::shared_ptr<phy::PhysicsEngine> physicsEngine = nullptr;
			};

			struct RobotStage {
				RobotProcessing robotProcessing = ROBOT_PROCESSING_SEQUENTIAL;
				std::function<void(void)> runAfterRobots;
			};

			struct RenderingStage {
				std::shared_ptr<vk::VulkanCore> vkCore;
				std::vector<std::shared_ptr<Renderer>> renderers;
			};

			struct PipelineSetup {
				GeneralConfig generalConfig;
				PhysicsStage physicsStage;	
				RobotStage robotStage;	
				RenderingStage renderingStage;	
			};

			ThreadManager(PipelineSetup pipelineSetup);
			~ThreadManager();

			void run();

		private:
			void createGeneralistWorkers();
			void createGuiWorker();

			void createCoreObjects();
			void createPhysicsObjects();
			void createRenderingObjects();

			bool _shouldFinish;

			//---------- Parallel ----------//
			// Syncronization structures
			std::shared_ptr<Barrier> _setupStageBarrier;
			std::shared_ptr<Barrier> _physicsStageBarrier;
			std::shared_ptr<Barrier> _robotStageBarrier;
			std::shared_ptr<Barrier> _renderingStageBarrier;

			// Threads
			std::vector<std::thread> _threads;
			// Workers
			unsigned _qtyWorkersToCreate;
			std::vector<std::shared_ptr<WorkerGeneralist>> _workersGen;
			std::shared_ptr<WorkerGui> _workerGui;

			//---------- Core ----------//
			std::shared_ptr<Scene> _scene;
			std::shared_ptr<Accelerator> _accelerator;
			DimMode _dimensionMode;

			//---------- Physics stage ----------//
			std::shared_ptr<phy::PhysicsEngine> _physicsEngine;

			//---------- Robot stage ----------//
			RobotProcessing _robotProcessing;
			std::function<void(void)> _runAfterRobots;
			
			//---------- Rendering stage ----------//
			std::shared_ptr<vk::VulkanCore> _vkCore;
			std::shared_ptr<vk::CommandPool> _commandPool;
			std::vector<std::shared_ptr<Renderer>> _renderers;
	};
}

#endif// ATTA_PARALLEL_THREAD_MANAGER_H
