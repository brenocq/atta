//--------------------------------------------------
// Atta Physics
// gravityForce.h
// Date: 2020-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/physics/forces/gravityForce.h>

namespace atta::phy
{
	GravityForce::GravityForce(vec3 gravity):
		_gravity(gravity)
	{

	}

	GravityForce::~GravityForce()
	{

	}

	void GravityForce::updateForce(std::shared_ptr<Body> object, float dt)
	{
		// Check infinity mass
		if(object->getInverseMass()==0)
			return;

		// Apply gravity
		object->addForce(_gravity*object->getMass());
	}
}
