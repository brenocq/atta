//--------------------------------------------------
// Atta Physics
// boxShape.h
// Date: 2020-12-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_SHAPES_BOX_SHAPE_H
#define ATTA_PHYSICS_SHAPES_BOX_SHAPE_H

#include "shape.h"

namespace atta::phy
{
	class BoxShape : public Shape
	{
		public:
			BoxShape(vec3 position = vec3(), quat orientation = quat(), vec3 size = vec3(1,1,1));
			~BoxShape();

			vec3 getSize() const { return _scale; }
			mat3 calculateInertiaTensor(float mass);

		private:
	};
}

#endif// ATTA_PHYSICS_SHAPES_BOX_SHAPE_H
