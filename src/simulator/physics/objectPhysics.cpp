//--------------------------------------------------
// Robot Simulator
// objectPhysics.cpp
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "objectPhysics.h"

ObjectPhysics::ObjectPhysics(btCollisionShape* bulletShape, glm::vec3 position, glm::vec3 rotation, float mass):
	_mass(mass)
{
	_bulletShape = bulletShape;

	// Create the initial transform
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(PhysicsEngine::glm2bt(position));
	transform.setRotation(PhysicsEngine::glm2bt(glm::quat(rotation)));
	_bulletMotionState = new btDefaultMotionState(transform);

	// Calculate inertia
	btVector3 localInertia(0,0,0);	
	if(_mass != 0.0f)
		_bulletShape->calculateLocalInertia(_mass, localInertia);

	// Create rigid body
	btRigidBody::btRigidBodyConstructionInfo cInfo(_mass, _bulletMotionState, _bulletShape, localInertia);
	_bulletBody = new btRigidBody(cInfo);
}

ObjectPhysics::~ObjectPhysics()
{
	if(_bulletShape!=nullptr)
	{
		delete _bulletShape;
		_bulletShape = nullptr;
	}

	if(_bulletBody!=nullptr)
	{
		delete _bulletBody;
		_bulletBody = nullptr;
	}

	if(_bulletMotionState!=nullptr)
	{
		delete _bulletMotionState;
		_bulletMotionState = nullptr;
	}
}
