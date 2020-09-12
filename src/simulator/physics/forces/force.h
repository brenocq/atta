//--------------------------------------------------
// Robot Simulator
// force.h
// Date: 2020-09-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef FORCE_H
#define FORCE_H

#include "glm.h"
#include "../objectPhysics.h"

class Force
{
	public:
		// Overload this method to update the force applied to an object
		virtual void updateForce(ObjectPhysics* object, float dt);

	private:
};

#endif// FORCE_H
