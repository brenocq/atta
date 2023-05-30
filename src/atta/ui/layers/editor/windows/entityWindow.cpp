//--------------------------------------------------
// Atta UI Module
// entityWindow.cpp
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/registry/registry.h>
#include <atta/component/components/components.h>
#include <atta/component/dataManager/cpuDataManager.h>
#include <atta/resource/interface.h>
#include <atta/resource/resources/mesh.h>
#include <atta/script/manager.h>
#include <atta/sensor/manager.h>
#include <atta/ui/layers/editor/windows/entityWindow.h>
#include <imgui.h>

namespace atta::ui {

EntityWindow::EntityWindow() {}

void EntityWindow::render() {
    ImGui::Begin("Scene");
    {
        if (ImGui::BeginDragDropTarget()) {
            if (ImGui::AcceptDragDropPayload("component::EntityId"))
                LOG_DEBUG("ui::EntityWindow", "Drop outside");
            ImGui::EndDragDropTarget();
        }

        renderTree();
        ImGui::Separator();
        renderComponents();
    }
    ImGui::End();
}

void EntityWindow::renderTree() {
    std::vector<component::Entity> entities = component::getEntitiesView();
    int i = 0;

    ImGui::Text("Scene");

    // Render root entities
    for (component::Entity entity : entities) {
        component::Relationship* r = entity.get<component::Relationship>();
        if (!r || (r && r->getParent() == -1))
            renderTreeNode(entity, i);
    }

    const float size = 100.0f;
    ImVec2 cursor = ImGui::GetCursorPos();
    ImVec2 window = ImGui::GetWindowSize();
    ImGui::SetCursorPos(ImVec2((window.x - size) * 0.5f, cursor.y));
    if (ImGui::Button("Create entity", ImVec2(size, 0)))
        component::createEntity();

    //----- Operations -----//
    for (auto entity : _entitiesToDelete)
        component::destroyEntity(entity);
    _entitiesToDelete.clear();

    for (auto entity : _entitiesToCopy) {
        component::Entity copied = component::copyEntity(entity);
        component::setSelectedEntity(copied);
    }
    _entitiesToCopy.clear();
}

void EntityWindow::renderTreeNode(component::Entity entity, int& i) {
    //----- Name -----//
    std::string name = "<Entity " + std::to_string(entity) + ">";
    component::Name* n = entity.get<component::Name>();
    component::Relationship* r = entity.get<component::Relationship>();
    if (n)
        name = n->name;

    //----- Selected -----//
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (entity == component::getSelectedEntity())
        nodeFlags |= ImGuiTreeNodeFlags_Selected;

    //----- Leaf/Node -----//
    if (r) {
        if (r->getChildren().size() == 0)
            nodeFlags |= ImGuiTreeNodeFlags_Leaf;
    } else
        nodeFlags |= ImGuiTreeNodeFlags_Leaf;
    bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)i++, nodeFlags, name.c_str());

    //----- Select entity -----//
    if (ImGui::IsItemClicked())
        component::setSelectedEntity(entity);

    //----- Popup -----//
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::Selectable("Delete"))
            _entitiesToDelete.push_back(entity);

        if (ImGui::Selectable("Copy"))
            _entitiesToCopy.push_back(entity);
        ImGui::EndPopup();
    }

    //----- Drag/Drop entities -----//
    if (ImGui::BeginDragDropSource()) {
        ImGui::SetDragDropPayload("component::EntityId", &entity, sizeof(component::EntityId));
        ImGui::Text(name.c_str());
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("component::EntityId")) {
            DASSERT(payload->DataSize == sizeof(component::EntityId), "Payload data should have same size as component::EntityId");
            component::EntityId childEntity = *(const component::EntityId*)payload->Data;
            component::Relationship::setParent(entity, childEntity);
        }
        ImGui::EndDragDropTarget();
    }

    //----- Render children -----//
    if (nodeOpen) {
        if (r && r->getChildren().size() > 0)
            for (auto child : r->getChildren())
                renderTreeNode(child, i);
        ImGui::TreePop();
    }
}

void EntityWindow::renderComponents() {
    component::Entity selected = component::getSelectedEntity();
    if (selected == -1)
        return;

    ImGui::Text("Components (EntityId: %d)", selected);

    // Render options to edit each component
    for (auto compReg : component::getComponentRegistries()) {
        void* component = component::cpuDataManager->getComponent(selected.getId(), compReg->getId());
        if (component != nullptr) {
            std::string name = compReg->getName();
            if (compReg->getId() != component::getId<component::Relationship>()) {
                bool open = true;
                if (ImGui::CollapsingHeader((name + "##Components" + name + "Header").c_str(), &open))
                    compReg->renderUI((component::Component*)component);
                if (!open)
                    component::cpuDataManager->removeComponent(selected.getId(), compReg->getId());
            }
        }
    }

    // Align button
    const float size = 100.0f;
    ImVec2 cursor = ImGui::GetCursorPos();
    ImVec2 window = ImGui::GetWindowSize();
    ImGui::SetCursorPos(ImVec2((window.x - size) * 0.5f, cursor.y));
    if (ImGui::Button("Add Component", ImVec2(size, 0)))
        ImGui::OpenPopup("Scene_ComponentAdd");

    if (ImGui::BeginPopup("Scene_ComponentAdd")) {
        for (auto compReg : component::getComponentRegistries()) {
            void* component = component::cpuDataManager->getComponent(selected.getId(), compReg->getId());
            if (component == nullptr) {
                if (compReg->getId() != component::getId<component::Relationship>()) {
                    std::string name = compReg->getName();
                    if (ImGui::Selectable((name + "##ComponentAdd" + name).c_str()))
                        component::cpuDataManager->addComponent(selected.getId(), compReg->getId());
                }
            }
        }
        ImGui::EndPopup();
    }
}

void EntityWindow::textureCombo(std::string comboId, StringId& sid) {
    std::vector<StringId> textures = resource::getResources<resource::Image>();

    if (sid == "Empty texture") {
        DASSERT(textures.size() > 0, "At least one texture should be loaded to the memory");
        sid = textures[0];
    }

    if (ImGui::BeginCombo(("Texture##" + comboId).c_str(), fs::path(sid.getString()).filename().string().c_str())) {
        for (size_t i = 0; i < textures.size(); i++) {
            if (ImGui::Selectable(fs::path(textures[i].getString()).filename().string().c_str(), sid == textures[i]))
                sid = textures[i];
            if (textures[i] == sid)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

} // namespace atta::ui
