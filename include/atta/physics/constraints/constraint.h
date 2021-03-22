//--------------------------------------------------
// Atta Physics
// constraint.h
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_CONSTRAINTS_CONSTRAINT_H
#define ATTA_PHYSICS_CONSTRAINTS_CONSTRAINT_H

#include <string>
#include <atta/physics/body.h>

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
			std::shared_ptr<Body> _objA;
			std::shared_ptr<Body> _objB;
	};
}

#endif// ATTA_PHYSICS_CONSTRAINTS_CONSTRAINT_H
