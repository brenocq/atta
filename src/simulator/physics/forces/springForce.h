//--------------------------------------------------
// Robot Simulator
// springForce.h
// Date: 2020-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef SPRING_FORCE_H
#define SPRING_FORCE_H

#include "glm.h"
#include "force.h"
#include "../objectPhysics.h"

class SpringForce : public Force
{
	public:
		SpringForce(glm::vec3 connectionPoint, ObjectPhysics* other, glm::vec3 otherConnectionPoint, float k, float restLenght);
		~SpringForce();

		virtual void updateForce(ObjectPhysics* object, float dt);

	private:
		ObjectPhysics* _other;
		glm::vec3 _connectionPoint;
		glm::vec3 _otherConnectionPoint;
		float _k;
		float _restLenght;
};

#endif// SPRING_FORCE_H
