//--------------------------------------------------
// Atta Physics
// sphereShape.h
// Date: 2020-12-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_SHAPES_SPHERE_SHAPE_H
#define ATTA_PHYSICS_SHAPES_SPHERE_SHAPE_H

#include "shape.h"

namespace atta::phy
{
	class SphereShape : public Shape
	{
		public:
			SphereShape(vec3 position = vec3(), quat orientation = quat(), float radius = 1.0f);
			~SphereShape();

			float getRadius() const { return _radius; }
			mat3 calculateInertiaTensor(float mass);

		private:
			float _radius;
	};
}

#endif// ATTA_PHYSICS_SHAPES_SPHERE_SHAPE_H
