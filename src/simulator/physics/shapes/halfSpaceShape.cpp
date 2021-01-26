//--------------------------------------------------
// Atta Physics
// halfSpaceShape.h
// Date: 2021-01-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "halfSpaceShape.h"

namespace atta::phy
{
	HalfSpaceShape::HalfSpaceShape(vec3 normal, float offset):
		Shape(vec3(), quat(), vec3()), _normal(normal), _offset(offset)
	{
		setType(HALF_SPACE_SHAPE);
	}

	HalfSpaceShape::~HalfSpaceShape()
	{

	}
}
