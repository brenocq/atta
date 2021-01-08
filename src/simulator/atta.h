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
#include "simulator/parallel/threadPool.h"
#include "simulator/objects/object.h"
#include "simulator/graphics/core/window.h"
#include "simulator/graphics/renderer.h"
#include "simulator/scene.h"
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
			std::shared_ptr<ThreadPool> _threadPool;
			std::shared_ptr<Window> _window;
			std::shared_ptr<Renderer> _renderer;

			std::shared_ptr<Scene> _scene;
			//std::shared_ptr<Drawer> _drawer;
			//std::shared_ptr<Application> _vulkanApp;
	};
}

#endif// ATTA_SIMULATOR_H
