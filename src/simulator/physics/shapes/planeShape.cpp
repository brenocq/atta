//--------------------------------------------------
// Atta Physics
// planeShape.h
// Date: 2021-01-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "planeShape.h"

namespace atta::phy
{
	PlaneShape::PlaneShape(vec3 normal, float offset):
		Shape(vec3(), quat(), vec3()), _normal(normal), _offset(offset)
	{
		setType(PLANE_SHAPE);
		setTypeName("PLANE");
	}

	PlaneShape::~PlaneShape()
	{

	}
}
