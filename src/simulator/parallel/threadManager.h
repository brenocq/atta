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

namespace atta
{
	class ThreadManager
	{
		public:
			struct GeneralConfig {
				int qtyThreads = -1;
			};

			struct PhysicsStage {

			};

			struct RenderingStage {
				std::shared_ptr<vk::VulkanCore> vkCore;
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

			unsigned _qtyWorkersToCreate;

			// Syncronization structures
			std::shared_ptr<Barrier> _generalistStateBarrier;

			// Threads
			std::vector<std::thread> _threads;
			// Workers
			std::vector<std::shared_ptr<WorkerGeneralist>> _workersGen;
			std::shared_ptr<WorkerGui> _workerGui;

			// Physics phase
			
			// Rendering phase
			std::vector<std::shared_ptr<Renderer>> _renderers;

			// Robot phase
	};
}

#endif// ATTA_THREAD_MANAGER_H
