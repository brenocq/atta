//--------------------------------------------------
// Atta Physics
// capsuleShape.cpp
// Date: 2020-12-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "capsuleShape.h"

namespace atta::phy
{
	CapsuleShape::CapsuleShape(vec3 position, quat orientation, vec3 size):
		Shape(position, orientation, size)
	{
		setType(CAPSULE_SHAPE);
		setTypeName("CAPSULE");
	}

	CapsuleShape::~CapsuleShape()
	{

	}
}
