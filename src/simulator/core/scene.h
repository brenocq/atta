//--------------------------------------------------
// Atta Robot Simulator
// scene.h
// Date: 2021-01-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCENE_H
#define ATTA_SCENE_H

#include <vector>
#include <memory>
#include "simulator/objects/object.h"
#include "simulator/core/robot.h"

namespace atta
{
	class Scene
	{
		public:
			struct CreateInfo
			{
				std::vector<std::shared_ptr<Object>> objects = {};
				std::vector<std::shared_ptr<Robot>> robots = {};
			};

			Scene(CreateInfo info);
			~Scene();

			//---------- Getters ----------//
			std::vector<std::shared_ptr<Object>> getObjects() const { return _objects; }
			std::vector<std::shared_ptr<Robot>> getRobots() const { return _robots; }

		private:
			std::vector<std::shared_ptr<Object>> _objects;
			std::vector<std::shared_ptr<Robot>> _robots;
	};
}

#endif// ATTA_SCENE_H
