//--------------------------------------------------
// Robot Simulator
// forceGenerator.h
// Date: 2020-09-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef FORCE_GENERATOR_H
#define FORCE_GENERATOR_H

#include "force.h"
#include "../objectPhysics.h"
#include <vector>

class ForceGenerator
{
	public:
		struct ForceRegistration
		{
			ObjectPhysics* object;
			Force* force;
		};

		ForceGenerator();
		~ForceGenerator();

		void add(ObjectPhysics* object, Force* force);
		void remove(ObjectPhysics* object, Force* force);
		void clear();
		
		void updateForces(float dt);

	private:
		std::vector<ForceRegistration> _registrations;
		
};

#endif// FORCE_GENERATOR_H
