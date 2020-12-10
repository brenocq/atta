//--------------------------------------------------
// Atta Physics
// force.h
// Date: 2020-09-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHY_FORCE_H
#define ATTA_PHY_FORCE_H

#include "glm.h"
#include "simulator/physics/body.h"

namespace atta::phy
{
	class Force
	{
		public:
			// Overload this method to update the force applied to an object
			virtual void updateForce(Body* object, float dt) = 0;

		private:
	};
}
#endif// ATTA_PHY_FORCE_H
