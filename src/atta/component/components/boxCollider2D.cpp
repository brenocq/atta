//--------------------------------------------------
// Atta Component Module
// boxCollider2D.cpp
// Date: 2022-10-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/boxCollider2D.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<BoxCollider2D>::getDescription() {
    static ComponentDescription desc = {
        "Box Collider 2D",
        {
            {AttributeType::FLOAT32, offsetof(BoxCollider2D, size), "size", 0.0001f, 2000.0f, 0.01f},
            {AttributeType::FLOAT32, offsetof(BoxCollider2D, offset), "offset", -2000.0f, 2000.0f, 0.01f},
        },
    };

    return desc;
}

} // namespace atta::component
