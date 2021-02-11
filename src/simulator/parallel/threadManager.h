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
#include "simulator/graphics/renderers/rayTracing/rayTracingVulkan/rayTracing.h"
#include "simulator/graphics/vulkan/vulkanCore.h"
#include "simulator/physics/physicsEngine.h"

namespace atta
{
	class ThreadManager
	{
		public:
			enum DimMode
			{
				DIM_MODE_2D = 0,
				DIM_MODE_3D
			};

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

			void populateThreadsWork();

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
			
			//---------- Rendering stage ----------//
			std::shared_ptr<vk::VulkanCore> _vkCore;
			std::vector<std::shared_ptr<Renderer>> _renderers;

			//---------- Robot stage ----------//
	};
}

#endif// ATTA_THREAD_MANAGER_H
