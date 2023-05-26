//--------------------------------------------------
// Atta Component Module
// mesh.cpp
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/mesh.h>
#include <atta/resource/interface.h>
#include <atta/resource/resources/mesh.h>
#include <imgui.h>

namespace atta::component {

void meshRenderImGui(void* data, std::string imguiId) {
    Mesh* mesh = static_cast<Mesh*>(data);
    resource::Mesh* m = resource::get<resource::Mesh>(mesh->sid.getString());
    if (m == nullptr)
        return;

    std::string selectedName = mesh->sid.getString();

    //---------- Selection ----------//
    if (ImGui::BeginCombo(("##Combo" + imguiId).c_str(), selectedName.c_str())) {
        std::vector<StringId> rMeshes = resource::getResources<resource::Mesh>();
        for (StringId rMesh : rMeshes) {
            std::string meshStr = rMesh.getString();
            if (meshStr == "")
                meshStr = "##";
            const bool selected = (rMesh == mesh->sid);
            if (ImGui::Selectable(meshStr.c_str(), selected))
                mesh->sid = rMesh;
            if (selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

template <>
ComponentDescription& TypedRegistry<Mesh>::getDescription() {
    static ComponentDescription desc = {"Mesh",
                                        {
                                            {AttributeType::STRINGID, offsetof(Mesh, sid), "sid", {}, {}, {}, {}},
                                        },
                                        1024, // Max instances
                                        {},   // Serialize
                                        {},   // Deserialize
                                        {     // renderUI
                                         {
                                             "", meshRenderImGui // Define how the component will be rendered
                                         }}};

    return desc;
}

Mesh::Mesh() {
    sid = *resource::getResources<resource::Mesh>().begin();
}

void Mesh::set(std::string mesh) { sid = StringId(mesh); }

} // namespace atta::component
