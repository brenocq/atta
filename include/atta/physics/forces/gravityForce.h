//--------------------------------------------------
// Atta Physics
// gravityForce.h
// Date: 2020-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_FORCES_GRAVITY_FORCE_H
#define ATTA_PHYSICS_FORCES_GRAVITY_FORCE_H

#include "force.h"

namespace atta::phy
{
	class GravityForce : public Force
	{
		public:
			GravityForce(vec3 gravity);
			~GravityForce();

			virtual void updateForce(std::shared_ptr<Body> object, float dt);

		private:
			vec3 _gravity;
	};
}

#endif// ATTA_PHYSICS_FORCES_GRAVITY_FORCE_H
