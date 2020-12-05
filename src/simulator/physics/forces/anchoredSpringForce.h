//--------------------------------------------------
// Robot Simulator
// anchoredSpringForce.h
// Date: 2020-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ANCHORED_SPRING_FORCE_H
#define ANCHORED_SPRING_FORCE_H

#include "glm.h"
#include "force.h"
#include "../objectPhysics.h"

class AnchoredSpringForce : public Force
{
	public:
		AnchoredSpringForce(glm::vec3 anchor, float k, float restLenght);
		~AnchoredSpringForce();

		virtual void updateForce(ObjectPhysics* object, float dt);

	private:
		glm::vec3 _anchor;
		float _k;
		float _restLenght;
};

#endif// ANCHORED_SPRING_FORCE_H
