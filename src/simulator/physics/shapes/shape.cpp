//--------------------------------------------------
// Atta Physics
// shape.cpp
// Date: 2020-12-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "shape.h"

namespace atta::phy
{
	Shape::Shape(vec3 position, quat orientation, vec3 scale):
		_position(position), _orientation(orientation), _scale(scale)

	{

	}

	Shape::~Shape()
	{

	}
}
