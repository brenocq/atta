//--------------------------------------------------
// Robot Simulator
// shape.h
// Date: 2020-12-31
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "shape.h"

namespace atta
{
	bnd3 worldBound() const
	{
		return (*objectToWorld)(objectBound());
	}

}
