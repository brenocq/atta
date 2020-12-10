//--------------------------------------------------
// Atta Physics
// forceGenerator.h
// Date: 2020-09-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHY_FORCE_GENERATOR_H
#define ATTA_PHY_FORCE_GENERATOR_H

#include "force.h"
#include "simulator/physics/body.h"
#include <vector>

namespace atta::phy
{
	class ForceGenerator
	{
		public:
			struct ForceRegistration
			{
				Body* object;
				Force* force;
			};

			ForceGenerator();
			~ForceGenerator();

			void add(Body* object, Force* force);
			void remove(Body* object, Force* force);
			void clear();
			
			void updateForces(float dt);

		private:
			std::vector<ForceRegistration> _registrations;
			
	};
}
#endif// ATTA_PHY_FORCE_GENERATOR_H
