//--------------------------------------------------
// Atta Physics
// anchoredSpringForce.h
// Date: 2020-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHY_ANCHORED_SPRING_FORCE_H
#define ATTA_PHY_ANCHORED_SPRING_FORCE_H

#include "force.h"
#include "simulator/physics/body.h"

namespace atta::phy
{
	class AnchoredSpringForce : public Force
	{
		public:
			AnchoredSpringForce(vec3 anchor, float k, float restLenght);
			~AnchoredSpringForce();

			virtual void updateForce(std::shared_ptr<Body> object, float dt);

		private:
			vec3 _anchor;
			float _k;
			float _restLenght;
	};
}
#endif// ATTA_PHY_ANCHORED_SPRING_FORCE_H
