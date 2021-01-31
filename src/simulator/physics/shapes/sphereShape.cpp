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
		setTypeName("SPHERE");
	}

	SphereShape::~SphereShape()
	{

	}

	mat3 SphereShape::calculateInertiaTensor(float mass)
	{
		float c = 2.0f/5 * mass * _radius*_radius;
		mat3 inertiaTensor(c, 0, 0,
						   0, c, 0,
						   0, 0, c);

		return inertiaTensor;
	}
}
