//--------------------------------------------------
// Atta Physics
// springForce.h
// Date: 2020-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHY_SPRING_FORCE_H
#define ATTA_PHY_SPRING_FORCE_H

#include "glm.h"
#include "force.h"
#include "simulator/physics/body.h"

namespace atta::phy
{
	class SpringForce : public Force
	{
		public:
			SpringForce(glm::vec3 connectionPoint, Body* other, glm::vec3 otherConnectionPoint, float k, float restLenght);
			~SpringForce();

			virtual void updateForce(Body* object, float dt);

		private:
			Body* _other;
			glm::vec3 _connectionPoint;
			glm::vec3 _otherConnectionPoint;
			float _k;
			float _restLenght;
	};
}
#endif// ATTA_PHY_SPRING_FORCE_H
