//--------------------------------------------------
// Robot Simulator
// fixedConstraint.h
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef FIXED_CONSTRAINT_H
#define FIXED_CONSTRAINT_H
#include "constraint.h"

class FixedConstraint : public Constraint
{
	public:
		FixedConstraint(glm::vec3 position, glm::vec3 rotation);
		~FixedConstraint();

	private:
		glm::vec3 _position;
		glm::vec3 _rotation;
};

#endif// FIXED_CONSTRAINT_H
