//--------------------------------------------------
// Robot Simulator
// sphereShape.h
// Date: 2020-12-31
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "sphereShape.h"

namespace atta
{
	SphereShape::SphereShape()
	{

	}

	bnd3 SphereShape::objectBound() const
	{
		return bnd3(pnt3(-radius, -radius, -radius), pnt3(radius, radius, radius));
	}
}
