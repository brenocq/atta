//--------------------------------------------------
// Atta Physics
// shape.cpp
// Date: 2020-12-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/physics/shapes/shape.h>

namespace atta::phy
{
	Shape::Shape(vec3 position, quat orientation, vec3 scale):
		_body(nullptr), _position(position), _orientation(orientation), _scale(scale)

	{

	}

	Shape::~Shape()
	{

	}
}
