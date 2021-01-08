//--------------------------------------------------
// Robot Simulator
// scene.h
// Date: 2020-07-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCENE_H
#define ATTA_SCENE_H

#include <vector>
#include <memory>
#include "simulator/objects/object.h"

namespace atta
{
	class Scene
	{
		public:
			struct CreateInfo
			{
				std::vector<std::shared_ptr<Object>> objects;
			};

			Scene(CreateInfo info);
			~Scene();


		private:
			std::vector<std::shared_ptr<Object>> _objects;
	};
}

#endif// ATTA_SCENE_H
