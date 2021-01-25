//--------------------------------------------------
// Atta Physics
// fixedConstraint.cpp
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "fixedConstraint.h"

namespace atta::phy
{
	FixedConstraint::FixedConstraint(vec3 position, vec3 rotation):
		_position(position), _rotation(rotation)
	{
		_type = "FixedConstraint";
	}

	FixedConstraint::~FixedConstraint()
	{
	}
}
