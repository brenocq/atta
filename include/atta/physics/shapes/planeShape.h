//--------------------------------------------------
// Atta Physics
// planeShape.h
// Date: 2021-01-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_SHAPES_PLANE_SHAPE_H
#define ATTA_PHYSICS_SHAPES_PLANE_SHAPE_H

#include "shape.h"

namespace atta::phy
{
	class PlaneShape : public Shape
	{
		public:
			PlaneShape(vec3 position = vec3(), quat orientation = quat(), vec2 size = vec2(1,1));
			~PlaneShape();

			//---------- Getters ----------//
			mat3 calculateInertiaTensor(float mass) { return mat3(); }

		private:
	};
}

#endif// ATTA_PHYSICS_SHAPES_PLANE_SHAPE_H
