//--------------------------------------------------
// Atta Physics
// forceGenerator.h
// Date: 2020-09-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_FORCES_FORCE_GENERATOR_H
#define ATTA_PHYSICS_FORCES_FORCE_GENERATOR_H

#include "force.h"
#include <vector>
#include <memory>

namespace atta::phy
{
	class ForceGenerator
	{
		public:
			struct ForceRegistration
			{
				std::shared_ptr<Body> object;
				std::shared_ptr<Force> force;
			};

			ForceGenerator();
			~ForceGenerator();

			void add(std::shared_ptr<Body> object, std::shared_ptr<Force> force);
			void remove(std::shared_ptr<Body> object, std::shared_ptr<Force> force);
			void clear();
			
			void updateForces(float dt);

		private:
			std::vector<ForceRegistration> _registrations;
			
	};
}
#endif// ATTA_PHYSICS_FORCES_FORCE_GENERATOR_H
