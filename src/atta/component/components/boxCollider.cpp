// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/component/components/boxCollider.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<BoxCollider>::getDescription() {
    static ComponentDescription desc = {"Box Collider",
                                        {{AttributeType::VECTOR_FLOAT32, offsetof(BoxCollider, size), "size", 0.0001f, 2000.0f, 0.01f},
                                         {AttributeType::VECTOR_FLOAT32, offsetof(BoxCollider, offset), "offset", -2000.0f, 2000.0f, 0.01f}}};

    return desc;
}

} // namespace atta::component
