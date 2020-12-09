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


	_orientation = glm::quat();
	// TODO initialize inertia tensor from shape

	_forceAccum = glm::vec3(0,0,0);
}

ObjectPhysics::~ObjectPhysics()
{
}

void ObjectPhysics::addForce(glm::vec3 force)
{
	_forceAccum += force;
	_isAwake = true;
}

void ObjectPhysics::addForceAtBodyPoint(glm::vec3 force, glm::vec3 point)
{
	glm::vec3 worldPoint = getPointInWorldSpace(point);
	addForceAtPoint(force, worldPoint);
}

void ObjectPhysics::addForceAtPoint(glm::vec3 force, glm::vec3 point)
{
	// Convert point to relative to center of mass
	glm::vec3 pt = point - _position;

	_forceAccum += force;
	_torqueAccum += glm::cross(pt,force);

	_isAwake = true;
}

void ObjectPhysics::integrate(float dt)
{
	if(_inverseMass<=0)
		return;

	// Calculate linear acceleration
	glm::vec3 resultingAcc = _acceleration;
	resultingAcc += _forceAccum*_inverseMass;

	// Calculate angular acceleration
	glm::vec3 resultingAngAcc = _inverseInertiaTensor*_torqueAccum;;

	// Update linear velocity
	_velocity += resultingAcc*dt;
	_velocity *= powf(_damping, dt);

	// Update angular velocity
	_rotation += resultingAngAcc*dt;
	_rotation *= powf(_angularDamping, dt);

	// Update linear position
	_position += _velocity*dt;

	// TODO Update angular position
	//_orientation +=	_rotation*dt; 

	calculateDerivedData();
	clearAccumulators();
}

//------------------------------//
//----------- HELPERS ----------//
//------------------------------//
glm::vec3 ObjectPhysics::getPointInWorldSpace(glm::vec3 point)
{
	return glm::vec3(_transformMatrix*glm::vec4(point,1));
}

//------------------------------//
//----------- PRIVATE ----------//
//------------------------------//

void ObjectPhysics::clearAccumulators()
{
	_forceAccum = glm::vec3(0,0,0);
	_torqueAccum = glm::vec3(0,0,0);
}

void ObjectPhysics::calculateDerivedData()
{
	_orientation = glm::normalize(_orientation);


	// Calculate transform matrix
	glm::mat4 rotMat = glm::toMat4(_orientation);
	_transformMatrix = glm::translate(glm::mat4(1.0f), _position)*rotMat;

	// TODO Calculate inertia tensor in world space 

}
