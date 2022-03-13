//--------------------------------------------------
// Atta UI System
// physicsDrawer.cpp
// Date: 2021-12-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/uiSystem/layers/editor/drawers/physicsDrawer.h>
#include <atta/graphicsSystem/drawer.h>
#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/components/transformComponent.h>
#include <atta/componentSystem/components/boxColliderComponent.h>
#include <atta/componentSystem/components/sphereColliderComponent.h>

namespace atta::ui
{
	void PhysicsDrawer::update()
	{
		Drawer::clear<Drawer::Line>("atta::ui::PhysicsDrawer");

		std::vector<EntityId> entities = ComponentManager::getEntitiesView();
		for(auto entity : entities)
		{
			// Get transform
			auto t = ComponentManager::getEntityComponent<TransformComponent>(entity);
			if(!t) continue;
			vec3 position, scale;
			quat orientation;
			mat4 m = t->getWorldTransform(entity);
			m.getPosOriScale(position, orientation, scale);

			// Choose color
			vec4 color = { 0, 1, 0, 1 };

			// Draw box collider
			auto box = ComponentManager::getEntityComponent<BoxColliderComponent>(entity);
			if(box)
			{
				std::vector<vec3> vertices = {{0.5, 0.5, 0}, {-0.5, 0.5, 0}, {-0.5, -0.5, 0}, {0.5, -0.5, 0}};

				// Scale
				for(auto& v : vertices)
					v *= box->size;
				for(auto& v : vertices)
					v *= scale;

				// Rotate
				vec3 euler = orientation.toEuler();
				for(auto& v : vertices)
				{
					vec3 oldv = v;
					v.x = oldv.x*cos(-euler.z) - oldv.y*sin(-euler.z);
					v.y = oldv.x*sin(-euler.z) + oldv.y*cos(-euler.z);
				}

				// Translate
				for(auto& v : vertices)
					v += box->offset;
				for(auto& v : vertices)
					v += position;

				Drawer::add(Drawer::Line(vertices[0], vertices[1], color, color), "atta::ui::PhysicsDrawer");
				Drawer::add(Drawer::Line(vertices[1], vertices[2], color, color), "atta::ui::PhysicsDrawer");
				Drawer::add(Drawer::Line(vertices[2], vertices[3], color, color), "atta::ui::PhysicsDrawer");
				Drawer::add(Drawer::Line(vertices[3], vertices[0], color, color), "atta::ui::PhysicsDrawer");
			}

			// Draw circle collider
			auto circle = ComponentManager::getEntityComponent<SphereColliderComponent>(entity);
			if(circle)
			{
				const unsigned numVertices = 32;
				std::vector<vec3> vertices;
				for(unsigned i = 0; i < numVertices; i++)
				{
					float angle = i/float(numVertices) * 2*3.1415926535f;
					vertices.push_back(vec3(cos(angle), sin(angle), 0.0f));
				}

				// Scale
				for(auto& v : vertices)
					v *= circle->radius;
				for(auto& v : vertices)
					v *= std::max(scale.x, scale.y);

				// Translate
				for(auto& v : vertices)
					v += circle->offset;
				for(auto& v : vertices)
					v += position;

				for(unsigned i = 0; i < numVertices; i++)
					Drawer::add(Drawer::Line(vertices[i], vertices[(i-1)%numVertices], color, color), "atta::ui::PhysicsDrawer");
			}
		}
	}
}
