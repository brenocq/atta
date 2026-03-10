// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/component/components/rigidJoint.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<RigidJoint>::getDescription() {
    static ComponentDescription desc = {"Rigid Joint",
                                        {
                                            {AttributeType::INT32, offsetof(RigidJoint, bodyA), "bodyA"},
                                            {AttributeType::INT32, offsetof(RigidJoint, bodyB), "bodyB"},
                                        }};

    return desc;
}

} // namespace atta::component
