//--------------------------------------------------
// Robot Simulator
// constraint.h
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <string>
#include "../objectPhysics.h"

class Constraint
{
	public:
		Constraint();
		~Constraint();

		std::string getType() const { return _type; };

	protected:
		std::string _type;
		ObjectPhysics* _objA;
		ObjectPhysics* _objB;
};

#endif// CONSTRAINT_H
