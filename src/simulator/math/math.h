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
#include <limits>

namespace atta
{
	float radians(float degrees);
	vec3 radians(vec3 vec);
	float degrees(float radians);
	vec3 degrees(vec3 vec);

	static constexpr float maxFloat = std::numeric_limits<float>::max();
	static constexpr float infinity = std::numeric_limits<float>::infinity();
	static constexpr float machineEpsilon = std::numeric_limits<float>::epsilon()*0.5;
}

#endif// ATTA_MATH_H
