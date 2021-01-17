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
#include "simulator/parallel/threadManager.h"
#include "simulator/objects/object.h"
#include "simulator/graphics/vulkan/vulkanCore.h"
#include "simulator/core/accelerator.h"
#include "simulator/core/scene.h"
#include "simulator/graphics/renderers/renderer.h"
#include "simulator/graphics/renderers/rastRenderer/rastRenderer.h"
//#include "simulator/helpers/drawer.h"

namespace atta
{
	class Atta
	{
		public:
			struct CreateInfo
			{
				std::vector<std::shared_ptr<Object>> objects;
			};

			Atta(CreateInfo createInfo);
			~Atta();

			void run();

		private:
			std::shared_ptr<ThreadManager> _threadManager;

			std::shared_ptr<vk::VulkanCore> _vulkanCore;
			std::shared_ptr<Scene> _scene;
			std::shared_ptr<Accelerator> _accelerator;

			std::vector<std::shared_ptr<Renderer>> _renderers;

			//std::shared_ptr<Drawer> _drawer;
			//std::shared_ptr<Application> _vulkanApp;
	};
}

#endif// ATTA_SIMULATOR_H
