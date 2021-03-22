//--------------------------------------------------
// Atta Physics
// anchoredSpringForce.h
// Date: 2020-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/physics/forces/anchoredSpringForce.h>

namespace atta::phy
{
	AnchoredSpringForce::AnchoredSpringForce(vec3 anchor, float k, float restLenght):
		_anchor(anchor), _k(k), _restLenght(restLenght)
	{

	}

	AnchoredSpringForce::~AnchoredSpringForce()
	{

	}

	void AnchoredSpringForce::updateForce(std::shared_ptr<Body> object, float dt)
	{
		// Calculate anchoredSpring vector
		vec3 force = object->getPosition();
		force -= _anchor;

		// Calculate force magnitude
		float magnitude = force.length();
		magnitude = std::abs(magnitude-_restLenght)*_k;

		// Apply anchored spring force
		force.normalize();
		force *= -magnitude;
		object->addForce(force);
	}
}
