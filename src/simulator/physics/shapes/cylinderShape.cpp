//--------------------------------------------------
// Atta Physics
// cylinderShape.cpp
// Date: 2020-12-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "cylinderShape.h"

namespace atta::phy
{
	CylinderShape::CylinderShape(vec3 position, quat orientation, vec3 size):
		Shape(position, orientation, size)
	{
		setType(CYLINDER_SHAPE);
		setTypeName("CYLINDER");
	}

	CylinderShape::~CylinderShape()
	{

	}
}
