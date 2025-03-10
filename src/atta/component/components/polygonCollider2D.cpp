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
        // Serialize
        {{"points",
          [](std::ostream& os, void* data) {
              // std::vector<vec2>* points = static_cast<std::vector<vec2>*>(data);
              // file::write<uint32_t>(os, points->size());
              // for (vec2 p : *points) {
              //     file::write(os, p.x);
              //     file::write(os, p.y);
              // }
          }}},
        // Deserialize
        {{"points",
          [](std::istream& is, void* data) {
              // std::vector<vec2>* points = static_cast<std::vector<vec2>*>(data);
              // uint32_t size;
              // file::read(is, size);
              // for (int i = 0; i < size; i++) {
              //     vec2 p;
              //     file::read(is, p.x);
              //     file::read(is, p.y);
              //     points->push_back(p);
              // }
          }}},
    };

    return desc;
}

} // namespace atta::component
