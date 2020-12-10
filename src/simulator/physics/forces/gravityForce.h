//--------------------------------------------------
// Atta Physics
// gravityForce.h
// Date: 2020-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHY_GRAVITY_FORCE_H
#define ATTA_PHY_GRAVITY_FORCE_H

#include "glm.h"
#include "force.h"
#include "simulator/physics/body.h"

namespace atta::phy
{
	class GravityForce : public Force
	{
		public:
			GravityForce(glm::vec3 gravity);
			~GravityForce();

			virtual void updateForce(Body* object, float dt);

		private:
			glm::vec3 _gravity;
	};
}

#endif// ATTA_PHY_GRAVITY_FORCE_H
