//--------------------------------------------------
// Atta Robot Simulator
// scene.cpp
// Date: 2021-01-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/core/scene.h>
#include <atta/objects/lights/lights.h>
#include <queue>

namespace atta
{
	Scene::Scene(CreateInfo info):
		_objects(info.objects), _robots(info.robots)
	{
		//---------- Create flat objects list ----------//
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
			}
		}

		//---------- Process objects ----------//
		for(auto object : _objectsFlat)
		{
			//---------- Create lights from light objects ----------//
			// Light structs are sent to the GPU memory
			if(object->isLight())
			{
				std::string type = object->getType();
				if(type == "PointLight")
				{
					std::shared_ptr<PointLight> l = std::static_pointer_cast<PointLight>(object);
					_lights.push_back(Light::point(l->getPosition(), l->getIntensity()));
				}
				else if(type == "SpotLight")
				{
					std::shared_ptr<SpotLight> l = std::static_pointer_cast<SpotLight>(object);
					_lights.push_back(Light::spot(l->getPosition(), l->getDirection(), l->getIntensity(), l->getFalloffStart(), l->getTotalWidth()));
				}
				if(type == "DistantLight")
				{
					std::shared_ptr<DistantLight> l = std::static_pointer_cast<DistantLight>(object);
					_lights.push_back(Light::distant(l->getRadiance(), l->getDirection()));
				}
				if(type == "InfiniteLight")
				{
					std::shared_ptr<InfiniteLight> l = std::static_pointer_cast<InfiniteLight>(object);
					_lights.push_back(Light::infinite(l->getRadiance(), l->getPosition(), l->getOrientation(), l->getPrecomputedPower(), l->getWorldRadius(), l->getTextureIndex(), l->getPdfTextureIndex(), l->getIrradianceTextureIndex()));
				}
			}
		}
	}

	Scene::~Scene()
	{

	}
}
