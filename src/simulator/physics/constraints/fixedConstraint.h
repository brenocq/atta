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
			FixedConstraint(glm::vec3 position, glm::vec3 rotation);
			~FixedConstraint();

			glm::vec3 getPosition() const { return _position; };
			glm::vec3 getRotation() const { return _rotation; };

		private:
			glm::vec3 _position;
			glm::vec3 _rotation;
	};
}

#endif// ATTA_PHY_FIXED_CONSTRAINT_H
