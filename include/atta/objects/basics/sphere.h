//--------------------------------------------------
// Robot Simulator
// sphere.h
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_OBJECTS_BASICS_SPHERE_H
#define ATTA_OBJECTS_BASICS_SPHERE_H

#include <atta/objects/object.h>

namespace atta
{
	class Sphere : public Object
	{
		public:
			struct CreateInfo
			{
				std::string name = "Sphere";
				vec3 position = {0,0,0};
				float radius = 0.5f;
				float mass = 1.0f;
				Material material;
			};

			Sphere(CreateInfo info);
			~Sphere();

			//---------- Getters ----------//
			vec3 getColor() const { return _color; };

		private:
			vec3 _color;
	};
}

#endif// SPHERE_H
