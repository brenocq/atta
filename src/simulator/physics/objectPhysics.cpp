//--------------------------------------------------
// Robot Simulator
// objectPhysics.cpp
// Date: 2020-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "objectPhysics.h"

ObjectPhysics::ObjectPhysics(glm::vec3 position, glm::vec3 rotation, float mass):
	_position(position)
{
	if(mass > 0)
		_inverseMass = 1/mass;
	else
		_inverseMass = 0;

	_damping = 0.99;
	_velocity = glm::vec3(0,0,0);
	_acceleration = glm::vec3(0,0,0);
	_forceAccum = glm::vec3(0,0,0);
}

ObjectPhysics::~ObjectPhysics()
{
}

void ObjectPhysics::addForce(glm::vec3 force)
{
	_forceAccum += force;
}

void ObjectPhysics::integrate(float dt)
{
	if(_inverseMass<=0)
		return;

	// Update position
	_position += _velocity*dt;

	// Calculate acceleration
	glm::vec3 resultingAcc = _acceleration;
	resultingAcc += _forceAccum*_inverseMass;

	_velocity += resultingAcc*dt;
	_velocity *= powf(_damping, dt);

	clearAccumulator();
}

void ObjectPhysics::clearAccumulator()
{
	_forceAccum = glm::vec3(0,0,0);
}
