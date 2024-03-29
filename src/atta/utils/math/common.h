//--------------------------------------------------
// Atta Math
// common.h
// Date: 2020-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UTILS_MATH_COMMON_H
#define ATTA_UTILS_MATH_COMMON_H

#include <atta/utils/math/vector.h>

namespace atta {
float radians(float degrees);
vec3 radians(vec3 vec);
float degrees(float radians);
vec3 degrees(vec3 vec);

static constexpr float maxFloat = std::numeric_limits<float>::max();
static constexpr float infinity = std::numeric_limits<float>::infinity();
static constexpr float machineEpsilon = std::numeric_limits<float>::epsilon() * 0.5;
} // namespace atta

#endif // ATTA_UTILS_MATH_COMMON_H
