//--------------------------------------------------
// Atta Physics
// forceGenerator.cpp
// Date: 2020-09-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "forceGenerator.h"
#include <iostream>

namespace atta::phy
{
	ForceGenerator::ForceGenerator()
	{

	}

	ForceGenerator::~ForceGenerator()
	{

	}

	void ForceGenerator::add(Body* object, Force* force)
	{
		_registrations.push_back({object, force});
	}

	void ForceGenerator::remove(Body* object, Force* force)
	{

	}

	void ForceGenerator::clear()
	{

	}
			
	void ForceGenerator::updateForces(float dt)
	{
		for(auto r : _registrations)
		{
			r.force->updateForce(r.object, dt);
		}
	}
}
