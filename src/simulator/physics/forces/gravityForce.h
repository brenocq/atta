//--------------------------------------------------
// Robot Simulator
// gravityForce.h
// Date: 2020-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GRAVITY_FORCE_H
#define GRAVITY_FORCE_H

#include "glm.h"
#include "force.h"
#include "../objectPhysics.h"

class GravityForce : public Force
{
	public:
		GravityForce(glm::vec3 gravity);
		~GravityForce();

		virtual void updateForce(ObjectPhysics* object, float dt);

	private:
		glm::vec3 _gravity;
};

#endif// GRAVITY_FORCE_H
