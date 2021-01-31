//--------------------------------------------------
// Atta Physics
// boxShape.h
// Date: 2020-12-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "boxShape.h"

namespace atta::phy
{
	BoxShape::BoxShape(vec3 position, quat orientation, vec3 size):
		Shape(position, orientation, size)
	{
		setType(BOX_SHAPE);
		setTypeName("BOX");
	}

	BoxShape::~BoxShape()
	{

	}

	mat3 BoxShape::calculateInertiaTensor(float mass)
	{
		float dx = _scale.x*0.5;
		float dy = _scale.y*0.5;
		float dz = _scale.z*0.5;
		float c = 1.0f/12;
		mat3 inertiaTensor(c*mass*(dy*dy+dz*dz), 0, 0,
							0, c*mass*(dx*dx+dz*dz), 0,
							0, 0, c*mass*(dx*dx+dy*dy));

		return inertiaTensor;
	}
}
