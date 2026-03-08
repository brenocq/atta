// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/utils/math/vector.h>

namespace atta {
float radians(float degrees);
vec3 radians(vec3 vec);
float degrees(float radians);
vec3 degrees(vec3 vec);

// Wraps the angle in radians to the range [-PI, PI]
float wrapRadians(float angle);
// Wraps the angle in degrees to the range [-180, 180]
float wrapDegrees(float angle);

static constexpr float maxFloat = std::numeric_limits<float>::max();
static constexpr float infinity = std::numeric_limits<float>::infinity();
static constexpr float machineEpsilon = std::numeric_limits<float>::epsilon() * 0.5;
} // namespace atta
