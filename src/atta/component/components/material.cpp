//--------------------------------------------------
// Atta Component Module
// material.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/componentRegistry.h>
#include <atta/component/components/material.h>
#include <atta/resource/interface.h>
#include <atta/resource/resources/image.h>
#include <imgui.h>

namespace atta::component {

bool renderComboImage(std::string attribute, StringId& image) {
    bool isImage = (image != StringId());
    bool updated = false;
    std::string imguiId = attribute;
    if (isImage) {
        std::string selectedName = image.getString();
        if (ImGui::BeginCombo(("##ComboImage" + imguiId).c_str(), selectedName.c_str())) {
            std::vector<StringId> rImages = resource::getResources<resource::Image>();
            for (StringId rImage : rImages) {
                std::string imageStr = rImage.getString();
                if (imageStr == "")
                    imageStr = "##";
                const bool selected = (rImage == image);
                if (ImGui::Selectable(imageStr.c_str(), selected)) {
                    image = rImage;
                    updated = true;
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    if (ImGui::Checkbox(("Is image##IsImage" + imguiId).c_str(), &isImage)) {
        image = isImage ? "textures/white.png"_sid : StringId();
        updated = true;
    }
    return updated;
}

void materialRenderImGui(void* data, std::string imguiId) {
    Material* material = static_cast<Material*>(data);
    resource::Material* m = resource::get<resource::Material>(material->sid.getString());
    if (m == nullptr)
        return;

    std::string selectedName = material->sid.getString();

    //---------- Selection ----------//
    if (ImGui::BeginCombo(("##Combo" + imguiId).c_str(), selectedName.c_str())) {
        std::vector<StringId> rMaterials = resource::getResources<resource::Material>();
        for (StringId rMaterial : rMaterials) {
            std::string materialStr = rMaterial.getString();
            if (materialStr == "")
                materialStr = "##";
            const bool selected = (rMaterial == material->sid);
            if (ImGui::Selectable(materialStr.c_str(), selected))
                material->sid = rMaterial;
            if (selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    //---------- Create ----------//
    static bool warnDuplicated = false;
    ImGui::SameLine();
    if (ImGui::Button("+")) {
        ImGui::OpenPopup("Create material");
        warnDuplicated = false;
    }

    if (ImGui::BeginPopupModal("Create material")) {
        ImGui::Text("What will be the material name?");

        // Name input
        static char buf[64] = "Material";
        ImGui::InputText("##MaterialName", buf, sizeof(buf) / sizeof(char));

        // Warn if duplicated
        if (warnDuplicated)
            ImGui::Text("A material with this name already exists.");

        // Cancel button
        if (ImGui::Button("Cancel"))
            ImGui::CloseCurrentPopup();

        // Create button
        ImGui::SameLine();
        if (ImGui::Button("Create")) {
            std::string bufStr(buf);
            std::vector<StringId> matSids = resource::getResources<resource::Material>();

            // Check if is duplicated
            warnDuplicated = false;
            for (StringId matSid : matSids)
                if (matSid.getString() == bufStr) {
                    warnDuplicated = true;
                    break;
                }

            // If not duplicated, create
            if (!warnDuplicated) {
                resource::Material* created = resource::create<resource::Material>(bufStr, resource::Material::CreateInfo{});
                if (created)
                    material->sid = created->getId();
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }

    //---------- Delete ----------//
    ImGui::SameLine();
    if (ImGui::Button("-"))
        ImGui::OpenPopup("Delete material");
    if (ImGui::BeginPopupModal("Delete material")) {
        ImGui::Text("Are you sure you want to delete this material?");
        ImGui::Text("All component::Material that use this material\n"
                    "will lose the reference.");
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete")) {
            resource::destroy<resource::Material>(material->sid.getString());
            material->sid = "Material";
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    //---------- Edit ----------//
    ImGui::Text("Color");
    if (!m->colorIsImage()) {
        vec3 color = m->getColor();
        bool updated = false;
        if (ImGui::SliderFloat("R##ColorR", &color.x, 0.0f, 1.0f))
            updated = true;
        if (ImGui::SliderFloat("G##ColorG", &color.y, 0.0f, 1.0f))
            updated = true;
        if (ImGui::SliderFloat("B##ColorB", &color.z, 0.0f, 1.0f))
            updated = true;
        if (updated)
            m->setColor(color);
    }
    StringId colorImage = m->getColorImage();
    if (renderComboImage("Color", colorImage))
        m->setColorImage(colorImage);

    ImGui::Text("Roughness");
    if (!m->roughnessIsImage()) {
        float roughness = m->getRoughness();
        if (ImGui::SliderFloat("##Roughness", &roughness, 0.0f, 1.0f))
            m->setRoughness(roughness);
    }
    StringId roughnessImage = m->getRoughnessImage();
    if (renderComboImage("Roughness", roughnessImage))
        m->setRoughnessImage(roughnessImage);

    ImGui::Text("Metallic");
    if (!m->metallicIsImage()) {
        float metallic = m->getMetallic();
        if (ImGui::SliderFloat("##Metallic", &metallic, 0.0f, 1.0f))
            m->setMetallic(metallic);
    }
    StringId metallicImage = m->getMetallicImage();
    if (renderComboImage("Metallic", metallicImage))
        m->setMetallicImage(metallicImage);

    ImGui::Text("AO");
    if (!m->aoIsImage()) {
        float ao = m->getAo();
        if (ImGui::SliderFloat("##AO", &ao, 0.0f, 1.0f))
            m->setAo(ao);
    }
    StringId aoImage = m->getAoImage();
    if (renderComboImage("AO", aoImage))
        m->setAoImage(aoImage);
}

template <>
ComponentDescription& TypedComponentRegistry<Material>::getDescription() {
    static ComponentDescription desc = {"Material",
                                        {{AttributeType::STRINGID, offsetof(Material, sid), "sid", {}, {}, {}, {}}},
                                        1024, // Max instances
                                        {},   // Serialize
                                        {},   // Deserialize
                                        {     // renderUI
                                         {
                                             "", materialRenderImGui // Define how the component will be rendered
                                         }}};

    return desc;
}

resource::Material* Material::getResource() const { return resource::get<resource::Material>(sid.getString()); }
void Material::setResource(const resource::Material* material) { set(material); }

void Material::set(StringId material) { sid = material; }

void Material::set(const resource::Material* material) {
    if (material == nullptr) {
        LOG_WARN("component::Material", "Could not set resource because it is nullptr");
        return;
    }
    sid = material->getId();
}

} // namespace atta::component
