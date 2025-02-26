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
        // renderUI
        //{{"",
        //  [=](void* data, std::string imguiId) // Define how the component will be rendered
        //  {
        //      // Render offset
        //      const std::vector<AttributeDescription> aDescs = TypedComponentRegistry<PolygonCollider2D>::getDescription().attributeDescriptions;
        //      float size = aDescs[1].offset - aDescs[0].offset;
        //      void* attribData = (void*)((uint8_t*)data + aDescs[0].offset);
        //      ComponentRegistry::renderUIAttribute(aDescs[0], attribData, size, imguiId + aDescs[0].name);

        //      ImGui::Text("Points");
        //      std::vector<vec2>* points = (std::vector<vec2>*)((uint8_t*)data + aDescs[1].offset);
        //      for (int i = 0; i < points->size(); i++) {
        //          ImGui::PushID(i);
        //          {
        //              // Edit point
        //              ImGui::DragFloat2("##PolygonCollider2DPoint", (float*)&((*points)[i]));

        //              // Delete point
        //              ImGui::SameLine();
        //              if (ImGui::Button("-##PolygonCollider2DDeletePoint"))
        //                  points->erase(points->begin() + i);

        //              // Rearrange point
        //              if (i > 0) {
        //                  ImGui::SameLine();
        //                  if (ImGui::Button("^")) {
        //                      std::swap((*points)[i], (*points)[i - 1]);
        //                  }
        //              }
        //              if (i < points->size() - 1) {
        //                  ImGui::SameLine();
        //                  if (ImGui::Button("v")) {
        //                      std::swap((*points)[i], (*points)[i + 1]);
        //                  }
        //              }
        //          }
        //          ImGui::PopID();
        //      }
        //      // Add point
        //      if (ImGui::Button("+##PolygonCollider2DAddPoint"))
        //          points->push_back({});
        //  }}},
    };

    return desc;
}

} // namespace atta::component
