//--------------------------------------------------
// Atta Physics
// dragForce.h
// Date: 2020-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "dragForce.h"

namespace atta::phy
{
	DragForce::DragForce(float k1, float k2):
		_k1(k1), _k2(k2)
	{

	}

	DragForce::~DragForce()
	{

	}

	void DragForce::updateForce(std::shared_ptr<Body> object, float dt)
	{
		vec3 force = object->getVelocity();

		float dragCoeff = force.length();
		dragCoeff = _k1*dragCoeff + _k2*dragCoeff*dragCoeff; 

		force = normalize(force);
		force *= -dragCoeff;

		// Apply drag
		object->addForce(force);
	}
}
