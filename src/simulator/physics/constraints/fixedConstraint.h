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

		btTypedConstraint* createConstraint(ObjectPhysics* objA, ObjectPhysics* objB);

	private:
		glm::vec3 _position;
		glm::vec3 _rotation;

		btGeneric6DofConstraint* _constraint;
};

#endif// FIXED_CONSTRAINT_H
