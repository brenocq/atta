//--------------------------------------------------
// Robot Simulator
// springForce.h
// Date: 2020-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "springForce.h"

SpringForce::SpringForce(glm::vec3 connectionPoint, ObjectPhysics* other, glm::vec3 otherConnectionPoint, float k, float restLenght):
	_other(other), 
	_connectionPoint(connectionPoint), _otherConnectionPoint(otherConnectionPoint),
	_k(k), _restLenght(restLenght)
{

}

SpringForce::~SpringForce()
{

}

void SpringForce::updateForce(ObjectPhysics* object, float dt)
{
	// Local and other connection points in world space
	glm::vec3 lws = object->getPointInWorldSpace(_connectionPoint);
	glm::vec3 ows = _other->getPointInWorldSpace(_otherConnectionPoint);

	// Calculate spring vector
	glm::vec3 force = lws-ows;

	// Calculate force magnitude
	float magnitude = glm::length(force);
	magnitude = std::abs(magnitude-_restLenght);
	magnitude*=_k;

	// Apply spring force
	force = glm::normalize(force);
	force *= -magnitude;
	object->addForceAtPoint(force, lws);
}
