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
	}

	BoxShape::~BoxShape()
	{

	}
}
