//--------------------------------------------------
// Atta Robot Simulator
// scene.cpp
// Date: 2021-01-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/core/scene.h>
#include <queue>

namespace atta
{
	Scene::Scene(CreateInfo info):
		_objects(info.objects), _robots(info.robots)
	{
		//---------- Create object lists ----------//
		// Creating _objectsFlat
		// Creating _lights
		for(auto& rootObject : _objects)
		{
			std::queue<std::shared_ptr<Object>> objects;
			objects.push(rootObject);

			while(!objects.empty())
			{
				// Get next object
				std::shared_ptr<Object> object = objects.front();
				objects.pop();

				// Populate queue with its children
				std::vector<std::shared_ptr<Object>> children = object->getChildren();
				for(auto& child : children)
					objects.push(child);

				_objectsFlat.push_back(object);

				// Populate light objects
				if(object->isLight())
					_lights.push_back(object);
			}
		}
	}

	Scene::~Scene()
	{

	}
}
