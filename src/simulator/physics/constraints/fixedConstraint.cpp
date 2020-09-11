//--------------------------------------------------
// Robot Simulator
// fixedConstraint.cpp
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "fixedConstraint.h"

FixedConstraint::FixedConstraint(glm::vec3 position, glm::vec3 rotation):
	_position(position), _rotation(rotation)
{
	_type = "FixedConstraint";
}

FixedConstraint::~FixedConstraint()
{
}
