//--------------------------------------------------
// Atta Physics
// hingeConstraint.cpp
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/physics/constraints/hingeConstraint.h>

namespace atta::phy
{
	HingeConstraint::HingeConstraint(vec3 position, vec3 rotation):
		_position(position), _rotation(rotation)
	{
		_type = "HingeConstraint";
	}

	HingeConstraint::~HingeConstraint()
	{
	}
}
