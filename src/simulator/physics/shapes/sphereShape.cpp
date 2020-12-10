//--------------------------------------------------
// Atta Physics
// sphereShape.cpp
// Date: 2020-12-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "sphereShape.h"

namespace atta::phy
{
	SphereShape::SphereShape(vec3 position, float radius):
		Shape(position, quat(), vec3(1,1,1)), _radius(radius)
	{
		setType(SPHERE_SHAPE);
	}

	SphereShape::~SphereShape()
	{

	}
}
