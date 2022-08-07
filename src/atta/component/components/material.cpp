//--------------------------------------------------
// Atta Component Module
// material.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/componentRegistry.h>
#include <atta/component/components/material.h>
#include <atta/resource/manager.h>
#include <atta/resource/resources/image.h>
#include <imgui.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<Material>::getDescription() {
    static ComponentDescription desc = {
        "Material",
        {{AttributeType::VECTOR_FLOAT32, offsetof(Material, albedo), "albedo", 0.0f, 1.0f},
         {AttributeType::FLOAT32, offsetof(Material, metallic), "metallic", 0.0f, 1.0f},
         {AttributeType::FLOAT32, offsetof(Material, roughness), "roughness", 0.0f, 1.0f},
         {AttributeType::FLOAT32, offsetof(Material, ao), "ao", 0.0f, 1.0f},
         {AttributeType::STRINGID, offsetof(Material, albedoTexture), "albedoTexture"},
         {AttributeType::STRINGID, offsetof(Material, metallicTexture), "metallicTexture"},
         {AttributeType::STRINGID, offsetof(Material, roughnessTexture), "roughnessTexture"},
         {AttributeType::STRINGID, offsetof(Material, aoTexture), "aoTexture"},
         {AttributeType::STRINGID, offsetof(Material, normalTexture), "normalTexture"}},
        1024,                                      // Max instances
        {},                                        // Serialize
        {},                                        // Deserialize
        {                                          // renderUI
         {"", [=](void* data, std::string imguiId) // Define how the component will be rendered
          {
              const std::vector<AttributeDescription> aDescs = TypedComponentRegistry<Material>::getDescription().attributeDescriptions;
              Material* m = static_cast<Material*>(data);

              bool albedoFromImage = m->albedoTexture != "Empty texture"_sid;
              bool metallicFromImage = m->metallicTexture != "Empty texture"_sid;
              bool roughnessFromImage = m->roughnessTexture != "Empty texture"_sid;
              bool aoFromImage = m->aoTexture != "Empty texture"_sid;
              bool useNormal = m->normalTexture != "Empty texture"_sid;

              // albedo-metallic-roughness-ao
              std::vector<bool*> fromImage = {&albedoFromImage, &metallicFromImage, &roughnessFromImage, &aoFromImage};
              std::vector<StringId*> attribTextures = {&(m->albedoTexture), &(m->metallicTexture), &(m->roughnessTexture), &(m->aoTexture)};
              std::vector<StringId> textures = resource::Manager::getResources<resource::Image>();

              for (unsigned i = 0; i <= 3; i++) {
                  std::string name = aDescs[i].name;
                  if (!(*fromImage[i])) {
                      float size = aDescs[i + 1].offset - aDescs[i].offset;
                      void* attribData = (void*)((uint8_t*)data + aDescs[i].offset);
                      ComponentRegistry::renderUIAttribute<AttributeType::FLOAT32>(aDescs[i], attribData, size, imguiId + aDescs[i].name);
                  } else {
                      ImGui::Text(name.c_str());
                      // TODO search using texture selection utils window
                      // ImGui::SameLine();
                      // ImGui::Button("Change");

                      if (ImGui::BeginCombo(("Texture##combo" + attribTextures[i]->getString() + imguiId).c_str(),
                                            fs::path(attribTextures[i]->getString()).string().c_str())) {
                          for (size_t j = 0; j < textures.size(); j++) {
                              if (ImGui::Selectable(fs::path(textures[j].getString()).string().c_str(), *attribTextures[i] == textures[j]))
                                  *attribTextures[i] = textures[j];
                              if (textures[j] == *attribTextures[i])
                                  ImGui::SetItemDefaultFocus();
                          }
                          ImGui::EndCombo();
                      }
                  }

                  ImGui::Checkbox(("from image?##" + name + "Material").c_str(), fromImage[i]);
                  ImGui::Separator();
              }

              ImGui::Text("normal");
              ImGui::Checkbox("use##NormalMaterial", &useNormal);

              // Initialize if some checkbox changed
              // Plain color to image
              if (albedoFromImage && m->albedoTexture == "Empty texture"_sid)
                  m->albedoTexture = "textures/white.jpg"_sid;
              if (metallicFromImage && m->metallicTexture == "Empty texture"_sid)
                  m->metallicTexture = "textures/white.jpg"_sid;
              if (roughnessFromImage && m->roughnessTexture == "Empty texture"_sid)
                  m->roughnessTexture = "textures/white.jpg"_sid;
              if (aoFromImage && m->aoTexture == "Empty texture"_sid)
                  m->aoTexture = "textures/white.jpg"_sid;
              if (useNormal && m->normalTexture == "Empty texture"_sid)
                  m->normalTexture = "textures/white.jpg"_sid;

              // Image to plain color
              if (!albedoFromImage && m->albedoTexture != "Empty texture"_sid)
                  m->albedoTexture = "Empty texture"_sid;
              if (!metallicFromImage && m->metallicTexture != "Empty texture"_sid)
                  m->metallicTexture = "Empty texture"_sid;
              if (!roughnessFromImage && m->roughnessTexture != "Empty texture"_sid)
                  m->roughnessTexture = "Empty texture"_sid;
              if (!aoFromImage && m->aoTexture != "Empty texture"_sid)
                  m->aoTexture = "Empty texture"_sid;
              if (!useNormal && m->normalTexture != "Empty texture"_sid)
                  m->normalTexture = "Empty texture"_sid;
          }}}};

    return desc;
}

} // namespace atta::component
