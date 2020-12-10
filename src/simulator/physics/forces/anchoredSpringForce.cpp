//--------------------------------------------------
// Atta Physics
// anchoredSpringForce.h
// Date: 2020-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "anchoredSpringForce.h"

namespace atta::phy
{
	AnchoredSpringForce::AnchoredSpringForce(glm::vec3 anchor, float k, float restLenght):
		_anchor(anchor), _k(k), _restLenght(restLenght)
	{

	}

	AnchoredSpringForce::~AnchoredSpringForce()
	{

	}

	void AnchoredSpringForce::updateForce(Body* object, float dt)
	{
		// Calculate anchoredSpring vector
		glm::vec3 force = object->getPosition();
		force -= _anchor;

		// Calculate force magnitude
		float magnitude = glm::length(force);
		magnitude = std::abs(magnitude-_restLenght)*_k;

		// Apply anchored spring force
		force = glm::normalize(force);
		force *= -magnitude;
		object->addForce(force);
	}
}
