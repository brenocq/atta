//--------------------------------------------------
// Atta Physics
// springForce.h
// Date: 2020-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHY_SPRING_FORCE_H
#define ATTA_PHY_SPRING_FORCE_H

#include "force.h"
#include "simulator/physics/body.h"

namespace atta::phy
{
	class SpringForce : public Force
	{
		public:
			SpringForce(vec3 connectionPoint, std::shared_ptr<Body> other, vec3 otherConnectionPoint, float k, float restLenght);
			~SpringForce();

			virtual void updateForce(std::shared_ptr<Body> object, float dt);

		private:
			std::shared_ptr<Body> _other;
			vec3 _connectionPoint;
			vec3 _otherConnectionPoint;
			float _k;
			float _restLenght;
	};
}
#endif// ATTA_PHY_SPRING_FORCE_H
