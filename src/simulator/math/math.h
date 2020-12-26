//--------------------------------------------------
// Atta Math
// math.h
// Date: 2020-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MATH_H
#define ATTA_MATH_H

#include "vector.h"
#include "matrix.h"
#include "quaternion.h"

namespace atta
{
	float radians(float degrees);
	vec3 radians(vec3 vec);
	float degrees(float radians);
	vec3 degrees(vec3 vec);
}

#endif// ATTA_MATH_H
