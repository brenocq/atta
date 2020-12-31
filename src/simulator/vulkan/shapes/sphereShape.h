//--------------------------------------------------
// Robot Simulator
// sphereShape.h
// Date: 2020-12-31
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SPHERE_SHAPE_H
#define ATTA_SPHERE_SHAPE_H
#include "shape.h"

namespace atta
{
	class SphereShape : public Shape
	{
		public:
			SphereShape();
			bnd3 objectBound() const;

		private:
			const float radius;
	};
}

#endif// ATTA_SPHERE_SHAPE_H
