//--------------------------------------------------
// Robot Simulator
// hingeConstraint.cpp
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "hingeConstraint.h"

HingeConstraint::HingeConstraint(glm::vec3 position, glm::vec3 rotation):
	_position(position), _rotation(rotation), _constraint(nullptr)
{
	_type = "HingeConstraint";
}

HingeConstraint::~HingeConstraint()
{
	if(_constraint != nullptr)
	{
		delete _constraint;
		_constraint = nullptr;
	}
}

btTypedConstraint* HingeConstraint::createConstraint(ObjectPhysics* objA, ObjectPhysics* objB)
{
	// Constraint ends its creation when the scene links objects
	_objA = objA;
	_objB = objB;
	
	btTransform frameInA;
	frameInA = btTransform::getIdentity();
	frameInA.setOrigin(PhysicsEngine::glm2bt({0,0,0}));
	btTransform frameInB;
	frameInB = btTransform::getIdentity();
	frameInB.setOrigin(PhysicsEngine::glm2bt(_position));
	frameInB.setRotation(PhysicsEngine::glm2bt(glm::quat(glm::radians(_rotation))));

	_constraint = new btHingeConstraint(
			*objA->getRigidBody(), 
			*objB->getRigidBody(), 
			frameInA,
			frameInB);
	_constraint->setLimit(btScalar(-0.1), btScalar(0.1));

	return (btTypedConstraint*)_constraint;
}
