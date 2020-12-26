//--------------------------------------------------
// Atta Physics
// sphereShape.cpp
// Date: 2020-12-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "sphereShape.h"

namespace atta::phy
{
	SphereShape::SphereShape(vec3 position, quat orientation, float radius):
		Shape(position, orientation, vec3(radius*2,radius*2,radius*2)), _radius(radius)
	{
		setType(SPHERE_SHAPE);
	}

	SphereShape::~SphereShape()
	{

	}
}
