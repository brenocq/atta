//--------------------------------------------------
// Atta Physics
// hingeConstraint.h
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_CONSTRAINTS_HINGE_CONSTRAINT_H
#define ATTA_PHYSICS_CONSTRAINTS_HINGE_CONSTRAINT_H
#include "constraint.h"

namespace atta::phy
{
	class HingeConstraint : public Constraint
	{
		public:
			HingeConstraint(vec3 position, vec3 rotation);
			~HingeConstraint();

			vec3 getPosition() const { return _position; };
			vec3 getRotation() const { return _rotation; };

		private:
			vec3 _position;
			vec3 _rotation;
	};
}

#endif// ATTA_PHYSICS_CONSTRAINTS_HINGE_CONSTRAINT_H
