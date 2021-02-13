//--------------------------------------------------
// Atta Robot Simulator
// atta.h
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SIMULATOR_H
#define ATTA_SIMULATOR_H

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "simulator/parallel/threadManager.h"
#include "simulator/objects/object.h"
#include "simulator/core/robot.h"

namespace atta
{
	class Project;
	class Atta
	{
		public:
			enum DimMode 
			{
				DIM_MODE_2D = 0,
				DIM_MODE_3D
			};

			enum PhysicsMode 
			{
				PHY_MODE_DISABLED = 0,
				PHY_MODE_2D,
				PHY_MODE_3D
			};

			struct CreateInfo
			{
				DimMode dimensionMode = DIM_MODE_2D;
				PhysicsMode physicsMode = PHY_MODE_DISABLED;
				bool createWindow = true;
				std::vector<std::shared_ptr<Object>> objects = {};
				std::vector<std::shared_ptr<Robot>> robots = {};

				std::function<void(void)> runAfterRobots;
			};

			Atta(CreateInfo info);
			~Atta();
			static Atta createFromProject(Project project);

			void run();

		private:
			ThreadManager::GeneralConfig populateTMGeneralConfig();
			ThreadManager::PhysicsStage populateTMPhysicsStage();
			ThreadManager::RobotStage populateTMRobotStage();
			ThreadManager::RenderingStage populateTMRenderingStage();

			CreateInfo _info;
			std::shared_ptr<Scene> _scene;
			std::shared_ptr<ThreadManager> _threadManager;
	};
}

#endif// ATTA_SIMULATOR_H
