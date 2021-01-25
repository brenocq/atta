//--------------------------------------------------
// Atta Parallel
// threadManager.h
// Date: 2021-01-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_THREAD_MANAGER_H
#define ATTA_THREAD_MANAGER_H

#include <vector>
#include <thread>
#include <memory>
#include "barrier.h"
#include "workerGeneralist.h"
#include "workerGui.h"
#include "simulator/core/scene.h"
#include "simulator/core/accelerator.h"
#include "simulator/graphics/renderers/renderer.h"
#include "simulator/graphics/vulkan/vulkanCore.h"
#include "simulator/physics/physicsEngine.h"

namespace atta
{
	class ThreadManager
	{
		public:
			struct GeneralConfig {
				int qtyThreads = -1;
				std::shared_ptr<Scene> scene;
			};

			struct PhysicsStage {
				std::shared_ptr<Accelerator> accelerator;
				std::shared_ptr<phy::PhysicsEngine> physicsEngine;
			};

			struct RenderingStage {
				std::shared_ptr<vk::VulkanCore> vkCore;
				std::shared_ptr<Renderer> mainRenderer;
			};

			struct RobotStage {
				
			};

			struct PipelineSetup {
				GeneralConfig generalConfig;
				PhysicsStage physicsStage;	
				RenderingStage renderingStage;	
				RobotStage robotStage;	
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

			void populateThreadsWork();

			//---------- Parallel ----------//
			// Syncronization structures
			std::shared_ptr<Barrier> _setupStageBarrier;
			std::shared_ptr<Barrier> _physicsStageBarrier;
			std::shared_ptr<Barrier> _renderingStageBarrier;
			std::shared_ptr<Barrier> _robotStageBarrier;

			// Threads
			std::vector<std::thread> _threads;
			// Workers
			unsigned _qtyWorkersToCreate;
			std::vector<std::shared_ptr<WorkerGeneralist>> _workersGen;
			std::shared_ptr<WorkerGui> _workerGui;

			//---------- Core ----------//
			std::shared_ptr<Scene> _scene;
			std::shared_ptr<Accelerator> _accelerator;

			//---------- Physics stage ----------//
			std::shared_ptr<phy::PhysicsEngine> _physicsEngine;
			
			//---------- Rendering stage ----------//
			std::shared_ptr<vk::VulkanCore> _vkCore;
			std::vector<std::shared_ptr<Renderer>> _renderers;

			//---------- Robot stage ----------//
	};
}

#endif// ATTA_THREAD_MANAGER_H
