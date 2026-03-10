// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#include <atta/component/base.h>
#include <atta/component/interface.h>

namespace atta::component {

/// Joint to keep entities fixed to each other
/** The relative position and orientation between the entities will not change */
struct RigidJoint : public Component {
    EntityId bodyA; ///< First attached body
    EntityId bodyB; ///< Second attached body
};
ATTA_REGISTER_COMPONENT(RigidJoint)
template <>
ComponentDescription& TypedComponentRegistry<RigidJoint>::getDescription();

} // namespace atta::component
