//--------------------------------------------------
// Robot Simulator
// springForce.h
// Date: 2020-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "springForce.h"

SpringForce::SpringForce(ObjectPhysics* other, float k, float restLenght):
	_other(other), _k(k), _restLenght(restLenght)
{

}

SpringForce::~SpringForce()
{

}

void SpringForce::updateForce(ObjectPhysics* object, float dt)
{
	// Calculate spring vector
	glm::vec3 force = object->getPosition();
	force -= _other->getPosition();

	// Calculate force magnitude
	float magnitude = glm::length(force);
	magnitude = std::abs(magnitude-_restLenght);
	magnitude*=_k;

	// Apply spring force
	force = glm::normalize(force);
	force *= -magnitude;
	object->addForce(force);
}
