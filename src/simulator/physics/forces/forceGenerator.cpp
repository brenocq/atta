//--------------------------------------------------
// Robot Simulator
// forceGenerator.cpp
// Date: 2020-09-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "forceGenerator.h"

ForceGenerator::ForceGenerator()
{

}

ForceGenerator::~ForceGenerator()
{

}

void ForceGenerator::add(ObjectPhysics* object, Force* force)
{

}

void ForceGenerator::remove(ObjectPhysics* object, Force* force)
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
