//--------------------------------------------------
// Robot Simulator
// cylinder.h
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_OBJECTS_BASICS_CYLINDER_H
#define ATTA_OBJECTS_BASICS_CYLINDER_H

#include <atta/objects/object.h>

namespace atta
{
	class Cylinder : public Object
	{
		public:
			struct CreateInfo
			{
				std::string name = "Cylinder";
				vec3 position = {0,0,0};
				vec3 rotation = {0,0,0};
				float height = 1.0f;
				float radius = 0.5f;
				float mass = 1.0f;
				Material material;
			};

			Cylinder(CreateInfo info);
			~Cylinder();

			//---------- Getters ----------//
			vec3 getColor() const { return _color; };

		private:
			vec3 _color;

	};
}

#endif// ATTA_OBJECTS_BASICS_CYLINDER_H
