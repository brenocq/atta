//--------------------------------------------------
// Robot Simulator
// fixedConstraint.cpp
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "fixedConstraint.h"

FixedConstraint::FixedConstraint(glm::vec3 position, glm::vec3 rotation):
	_position(position), _rotation(rotation), _constraint(nullptr)
{
	_type = "FixedConstraint";
}

FixedConstraint::~FixedConstraint()
{
	if(_constraint != nullptr)
	{
		delete _constraint;
		_constraint = nullptr;
	}
}

btTypedConstraint* FixedConstraint::createConstraint(ObjectPhysics* objA, ObjectPhysics* objB)
{
	// Constraint ends its creation when the scene links objects
	_objA = objA;
	_objB = objB;
	
	btTransform frameInA;
	frameInA = btTransform::getIdentity();
	btTransform frameInB;
	frameInB = btTransform::getIdentity();
	frameInB.setOrigin(PhysicsEngine::glm2bt(_position));

	_constraint = new btGeneric6DofConstraint(
			*objA->getRigidBody(), 
			*objB->getRigidBody(), 
			frameInA,
			frameInB,
			true);

	// lock all rotations
	_constraint->setAngularLowerLimit(btVector3(0.f, 0.f, 0.f));
	_constraint->setAngularUpperLimit(btVector3(0.f, 0.f, 0.f));
	_constraint->setLinearLowerLimit(btVector3(0.f, 0.f, 0.f));
	_constraint->setLinearUpperLimit(btVector3(0.f, 0.f, 0.f));

	return (btTypedConstraint*)_constraint;
}
