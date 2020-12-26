//--------------------------------------------------
// Atta Math
// math.cpp
// Date: 2020-12-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "simulator/math/math.h"

namespace atta
{
	float radians(float degrees)
	{
		return degrees*0.01745329251f;
	}

	vec3 radians(vec3 vec)
	{
		return vec*0.01745329251f;
	}
	
	float degrees(float radians)
	{
		return radians*57.2957795147f;
	}

	vec3 degrees(vec3 vec)
	{
		return vec*57.2957795147f;
	}
}
