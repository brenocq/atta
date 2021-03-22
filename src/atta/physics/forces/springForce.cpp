//--------------------------------------------------
// Atta Physics
// springForce.h
// Date: 2020-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/physics/forces/springForce.h>

namespace atta::phy
{
	SpringForce::SpringForce(vec3 connectionPoint, std::shared_ptr<Body> other, vec3 otherConnectionPoint, float k, float restLenght):
		_other(other), 
		_connectionPoint(connectionPoint), _otherConnectionPoint(otherConnectionPoint),
		_k(k), _restLenght(restLenght)
	{

	}

	SpringForce::~SpringForce()
	{

	}

	void SpringForce::updateForce(std::shared_ptr<Body> object, float dt)
	{
		// Local and other connection points in world space
		vec3 lws = object->getPointInWorldSpace(_connectionPoint);
		vec3 ows = _other->getPointInWorldSpace(_otherConnectionPoint);

		// Calculate spring vector
		vec3 force = lws-ows;

		// Calculate force magnitude
		float magnitude = force.length();
		magnitude = std::abs(magnitude-_restLenght);
		magnitude*=_k;

		// Apply spring force
		force.normalize();
		force *= -magnitude;
		object->addForceAtPoint(force, lws);
	}
}
