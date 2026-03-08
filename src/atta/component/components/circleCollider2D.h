// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/component/components/component.h>
#include <atta/component/interface.h>
#include <atta/utils/math/vector.h>

namespace atta::component {

/// %Component to create sphere collider
/** This collider can be used with 2D physics.
 *
 * Transform and RigidBody2D components are necessary for the
 * entity to participate in the physics iteration.
 *
 * The circle will also be scaled by the transform scale. The final
 * radius will be:
 *
 * ```cpp
 * finalRadius = radius * max(transform.worldScale.x, transform.worldScale.y);
 * ```
 */
struct CircleCollider2D final : public Component {
    float radius = 0.5f;        ///< Radius
    vec2 offset = {0.0f, 0.0f}; ///< Offset
};
ATTA_REGISTER_COMPONENT(CircleCollider2D)
template <>
ComponentDescription& TypedComponentRegistry<CircleCollider2D>::getDescription();

} // namespace atta::component
