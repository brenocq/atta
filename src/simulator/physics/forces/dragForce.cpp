//--------------------------------------------------
// Robot Simulator
// dragForce.h
// Date: 2020-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "dragForce.h"

DragForce::DragForce(float k1, float k2):
	_k1(k1), _k2(k2)
{

}

DragForce::~DragForce()
{

}

void DragForce::updateForce(ObjectPhysics* object, float dt)
{
	glm::vec3 force = object->getVelocity();

	float dragCoeff = glm::length(force);
	dragCoeff = _k1*dragCoeff + _k2*dragCoeff*dragCoeff; 

	force = glm::normalize(force);
	force *= -dragCoeff;

	// Apply drag
	object->addForce(force);
}
