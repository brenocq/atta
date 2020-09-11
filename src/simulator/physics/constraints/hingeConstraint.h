//--------------------------------------------------
// Robot Simulator
// hingeConstraint.h
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef HINGE_CONSTRAINT_H
#define HINGE_CONSTRAINT_H
#include "constraint.h"

class HingeConstraint : public Constraint
{
	public:
		HingeConstraint(glm::vec3 position, glm::vec3 rotation);
		~HingeConstraint();

		btTypedConstraint* createConstraint(ObjectPhysics* objA, ObjectPhysics* objB);
		btHingeConstraint* getConstraint() const { return _constraint; }

	private:
		glm::vec3 _position;
		glm::vec3 _rotation;

		btHingeConstraint* _constraint;
};

#endif// HINGE_CONSTRAINT_H
