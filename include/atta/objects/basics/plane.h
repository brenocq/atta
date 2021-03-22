//--------------------------------------------------
// Robot Simulator
// plane.h
// Date: 2020-08-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_OBJECTS_BASICS_PLANE_H
#define ATTA_OBJECTS_BASICS_PLANE_H

#include <atta/objects/object.h>

namespace atta
{
	class Plane : public Object
	{
		public:
			struct CreateInfo
			{
				std::string name = "Plane";
				vec3 position = {0,0,0};
				vec3 rotation = {0,0,0};
				vec2 size = {1,1};
				float mass = 1.0f;
				Material material;
			};

			Plane(CreateInfo info);
			~Plane();

			//---------- Getters ----------//
			vec3 getColor() const { return _color; };

		private:
			vec3 _color;
	};
}

#endif// ATTA_OBJECTS_BASICS_PLANE_H
