//--------------------------------------------------
// Atta Component Module
// polygonCollider2D.cpp
// Date: 2022-10-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/polygonCollider2D.h>
#include <atta/file/serializer/serializer.h>
#include <imgui.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<PolygonCollider2D>::getDescription() {
    static ComponentDescription desc = {
        "Polygon Collider 2D",
        {
            {AttributeType::VECTOR_FLOAT32, offsetof(PolygonCollider2D, offset), "offset", -2000.0f, 2000.0f, 0.01f},
            {AttributeType::CUSTOM, offsetof(PolygonCollider2D, points), "points"},
        },
        // Max instances
        1024,
    };

    return desc;
}

} // namespace atta::component
