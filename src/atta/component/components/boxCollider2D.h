// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/component/interface.h>

namespace atta::component {

/// %Component to create a box collider
/** This collider can be used for 2D physics.
 *
 * Transform and RigidBody2D components are necessary
 * for the entity to participate in the physics iteration.
 *
 * The box will also be scaled by the Transform.
 */
struct BoxCollider2D final : public Component {
    vec2 size = {1.0f, 1.0f};   ///< Size
    vec2 offset = {0.0f, 0.0f}; ///< Offset
};
ATTA_REGISTER_COMPONENT(BoxCollider2D)
template <>
ComponentDescription& TypedComponentRegistry<BoxCollider2D>::getDescription();

} // namespace atta::component
