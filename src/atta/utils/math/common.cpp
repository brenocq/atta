//--------------------------------------------------
// Atta Math
// common.cpp
// Date: 2020-12-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/utils/math/common.h>

namespace atta {

float radians(float degrees) { return degrees * 0.01745329251f; }
vec3 radians(vec3 vec) { return vec * 0.01745329251f; }
float degrees(float radians) { return radians * 57.2957795147f; }
vec3 degrees(vec3 vec) { return vec * 57.2957795147f; }

float wrapRadians(float angle) {
    while (angle > M_PI)
        angle -= 2.0f * M_PI;
    while (angle < -M_PI)
        angle += 2.0f * M_PI;
    return angle;
}

float wrapDegrees(float angle) {
    while (angle > 180.0f)
        angle -= 360.0f;
    while (angle < -180.0f)
        angle += 360.0f;
    return angle;
}

} // namespace atta
