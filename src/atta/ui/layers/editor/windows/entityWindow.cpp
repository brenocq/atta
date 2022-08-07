//--------------------------------------------------
// Atta UI Module
// entityWindow.cpp
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/componentRegistry.h>
#include <atta/component/components/components.h>
#include <atta/resource/manager.h>
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
                LOG_DEBUG("EntityWindow", "Drop outside");
            ImGui::EndDragDropTarget();
        }

        renderTree();
        ImGui::Separator();
        renderComponents();
        renderCameraWindows();
    }
    ImGui::End();
}

void EntityWindow::renderTree() {
    std::vector<component::EntityId> entities = component::Manager::getEntitiesView();
    int i = 0;

    ImGui::Text("Scene");

    // Render root entities
    for (component::EntityId entity : entities) {
        component::Relationship* r = component::Manager::getEntityComponent<component::Relationship>(entity);
        if (!r || (r && r->getParent() == -1))
            renderTreeNode(entity, i);
    }

    const float size = 100.0f;
    ImVec2 cursor = ImGui::GetCursorPos();
    ImVec2 window = ImGui::GetWindowSize();
    ImGui::SetCursorPos(ImVec2((window.x - size) * 0.5f, cursor.y));
    if (ImGui::Button("Create entity", ImVec2(size, 0)))
        component::Manager::createEntity();

    //----- Operations -----//
    for (auto entity : _entitiesToDelete)
        component::Manager::deleteEntity(entity);
    _entitiesToDelete.clear();

    for (auto entity : _entitiesToCopy) {
        component::EntityId copied = component::Manager::copyEntity(entity);
        component::Manager::setSelectedEntity(copied);
    }
    _entitiesToCopy.clear();
}

void EntityWindow::renderTreeNode(component::EntityId entity, int& i) {
    //----- Name -----//
    std::string name = "<Entity " + std::to_string(entity) + ">";
    component::Name* n = component::Manager::getEntityComponent<component::Name>(entity);
    component::Relationship* r = component::Manager::getEntityComponent<component::Relationship>(entity);
    if (n)
        name = n->name;

    //----- Selected -----//
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (entity == component::Manager::getSelectedEntity())
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
        component::Manager::setSelectedEntity(entity);

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
    component::EntityId selected = component::Manager::getSelectedEntity();
    if (selected == -1)
        return;

    ImGui::Text("Components (component::EntityId: %d)", selected);

    // Render options to edit each component
    for (auto compReg : component::Manager::getComponentRegistries()) {
        void* component = component::Manager::getEntityComponentById(compReg->getId(), selected);
        if (component != nullptr) {
            std::string name = compReg->getDescription().name;
            if (compReg->getId() != component::TypedComponentRegistry<component::Relationship>::getInstance().getId()) {
                bool open = true;
                if (ImGui::CollapsingHeader((name + "##Components" + name + "Header").c_str(), &open))
                    compReg->renderUI((component::Component*)component);
                if (!open)
                    component::Manager::removeEntityComponentById(compReg->getId(), selected);
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
        for (auto compReg : component::Manager::getComponentRegistries()) {
            void* component = component::Manager::getEntityComponentById(compReg->getId(), selected);
            if (component == nullptr) {
                if (compReg->getId() != component::TypedComponentRegistry<component::Relationship>::getInstance().getId()) {
                    std::string name = compReg->getDescription().name;
                    if (ImGui::Selectable((name + "##ComponentAdd" + name).c_str()))
                        component::Manager::addEntityComponentById(compReg->getId(), selected);
                }
            }
        }
        ImGui::EndPopup();
    }
}

void EntityWindow::textureCombo(std::string comboId, StringId& sid) {
    std::vector<StringId> textures = resource::Manager::getResources<resource::Image>();

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

void EntityWindow::renderCameraWindows() {
    for (auto eid : _cameraWindows) {
        component::Camera* camera = component::Manager::getEntityComponent<component::Camera>(eid);
        bool open = true;
        ImGui::Begin(("Camera Entity " + std::to_string(eid)).c_str(), &open);
        ImVec2 size = ImVec2(camera->width, camera->height);
        ImGui::Image(sensor::Manager::getEntityCameraImGuiTexture(eid), size, ImVec2(0, 0), ImVec2(1, 1));
        ImGui::End();

        if (!open)
            _cameraWindows.erase(eid);
    }
}

} // namespace atta::ui
