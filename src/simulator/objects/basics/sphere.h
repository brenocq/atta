//--------------------------------------------------
// Robot Simulator
// sphere.h
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef SPHERE_H
#define SPHERE_H

#include "simulator/objects/object.h"

namespace atta
{
	class Sphere : public Object
	{
		public:
			struct CreateInfo
			{
				std::string name = "Sphere";
				vec3 position = {0,0,0};
				vec3 rotation = {0,0,0};
				float radius = 0.5f;
				float mass = 1.0f;
				vec3 color = {0.3f, 0.3f, 0.3f};
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
