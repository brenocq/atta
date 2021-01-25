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

	void ForceGenerator::add(std::shared_ptr<Body> object, std::shared_ptr<Force> force)
	{
		_registrations.push_back({object, force});
	}

	void ForceGenerator::remove(std::shared_ptr<Body> object, std::shared_ptr<Force> force)
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
