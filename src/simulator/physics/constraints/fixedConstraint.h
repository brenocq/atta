//--------------------------------------------------
// Atta Physics
// fixedConstraint.h
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHY_FIXED_CONSTRAINT_H
#define ATTA_PHY_FIXED_CONSTRAINT_H
#include "constraint.h"

namespace atta::phy
{
	class FixedConstraint : public Constraint
	{
		public:
			FixedConstraint(vec3 position, vec3 rotation);
			~FixedConstraint();

			vec3 getPosition() const { return _position; };
			vec3 getRotation() const { return _rotation; };

		private:
			vec3 _position;
			vec3 _rotation;
	};
}

#endif// ATTA_PHY_FIXED_CONSTRAINT_H
