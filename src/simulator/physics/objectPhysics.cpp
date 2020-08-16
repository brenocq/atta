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
	transform.setRotation(PhysicsEngine::glm2bt(glm::quat(glm::radians(rotation))));
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

glm::vec3 ObjectPhysics::getPosition() const 
{
	btTransform bulletTransform;
	btVector3 bulletPosition = {0,0,0};

	_bulletMotionState->getWorldTransform(bulletTransform);
	bulletPosition = bulletTransform.getOrigin();

	return PhysicsEngine::bt2glm(bulletPosition);
}

glm::vec3 ObjectPhysics::getRotation() const
{
	btTransform bulletTransform;
	btQuaternion bulletRotation = {0,0,0};

	_bulletMotionState->getWorldTransform(bulletTransform);
	bulletRotation = bulletTransform.getRotation();

	return glm::degrees(glm::eulerAngles(PhysicsEngine::bt2glm(bulletRotation)));
}
