// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/component/components/circleCollider2D.h>
#include <atta/component/components/transform.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<CircleCollider2D>::getDescription() {
    static ComponentDescription desc = {
        "Circle Collider 2D",
        {
            {AttributeType::FLOAT32, offsetof(CircleCollider2D, radius), "radius", 0.0001f, 2000.0f, 0.01f},
            {AttributeType::VECTOR_FLOAT32, offsetof(CircleCollider2D, offset), "offset", -2000.0f, 2000.0f, 0.01f},
        },
    };

    return desc;
}

} // namespace atta::component
