//--------------------------------------------------
// Atta Math
// quaternion.cpp
// Date: 2020-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "quaternion.h"

namespace atta
{
	void quat::transformVector(const vec3& before, const vec3& after)
	{
		vec3 rotAxis = cross(before, after);
		float cosTheta = dot(before, after);
		float halfAngle = acos(cosTheta)*0.5f;
		// Degenerated cases
		if(cosTheta==1)// Equal directions
			return;
		else if(cosTheta==-1)// Opposite directions
		{
			// Choose one of the tangent vectors
			if(before.x>before.y)
				rotAxis = cross(before, vec3(0,1,0));
			else
				rotAxis = cross(before, vec3(1,0,0));
		}
		rotAxis.normalize();
		float cosHalfAngle = cos(halfAngle);
		float sinHalfAngle = sin(halfAngle);

		// Create rotation quaternion
		r = cosHalfAngle;
		i = sinHalfAngle*rotAxis.x;
		j = sinHalfAngle*rotAxis.y;
		k = sinHalfAngle*rotAxis.z;
	}
}
