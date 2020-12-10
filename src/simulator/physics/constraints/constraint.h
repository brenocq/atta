//--------------------------------------------------
// Atta Physics
// constraint.h
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHY_CONSTRAINT_H
#define ATTA_PHY_CONSTRAINT_H

#include <string>
#include "simulator/physics/body.h"

namespace atta::phy
{
	class Constraint
	{
		public:
			Constraint();
			~Constraint();

			std::string getType() const { return _type; };

		protected:
			std::string _type;
			Body* _objA;
			Body* _objB;
	};
}

#endif// ATTA_PHY_CONSTRAINT_H
