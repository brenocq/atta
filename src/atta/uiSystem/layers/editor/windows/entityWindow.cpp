//--------------------------------------------------
// Atta UI System
// entityWindow.cpp
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/uiSystem/layers/editor/windows/entityWindow.h>
#include <atta/componentSystem/components/components.h>
#include <atta/componentSystem/componentRegistry.h>
#include <atta/scriptSystem/scriptManager.h>
#include <atta/resourceSystem/resourceManager.h>
#include <atta/resourceSystem/resources/mesh.h>
#include <atta/sensorSystem/sensorManager.h>
#include <imgui.h>

namespace atta::ui
{
    EntityWindow::EntityWindow()
    {
    }

    void EntityWindow::render()
    {
        ImGui::Begin("Scene");
        {
            renderTree();
            ImGui::Separator();
            renderComponents();
            renderCameraWindows();
        }
        ImGui::End();
    }

    void EntityWindow::renderTree()
    {
        std::vector<EntityId> entities = ComponentManager::getEntitiesView();
        int i = 0;
        ImGui::Text("Scene");

        // Render root entities
        for(EntityId entity : entities)
        {
            RelationshipComponent* r = ComponentManager::getEntityComponent<RelationshipComponent>(entity);
            if(!r || (r && r->getParent() == -1))
                renderTreeNode(entity, i);
        }

        const float size = 100.0f;
        ImVec2 cursor = ImGui::GetCursorPos();
        ImVec2 window = ImGui::GetWindowSize();
        ImGui::SetCursorPos(ImVec2((window.x - size)*0.5f, cursor.y));
        if(ImGui::Button("Create entity", ImVec2(size, 0)))
            ComponentManager::createEntity();

        //----- Operations -----//
        for(auto entity : _entitiesToDelete)
            ComponentManager::deleteEntity(entity);
        _entitiesToDelete.clear();

        for(auto entity : _entitiesToCopy)
        {
            EntityId copied = ComponentManager::copyEntity(entity);
            ComponentManager::setSelectedEntity(copied);
        }
        _entitiesToCopy.clear();
    }

    void EntityWindow::renderTreeNode(EntityId entity, int& i)
    {
        //----- Name -----//
        std::string name = "<Entity "+std::to_string(entity)+">";
        NameComponent* n = ComponentManager::getEntityComponent<NameComponent>(entity);
        RelationshipComponent* r = ComponentManager::getEntityComponent<RelationshipComponent>(entity);
        if(n) name = n->name;

        //----- Selected -----//
        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        if(entity == ComponentManager::getSelectedEntity())
            nodeFlags |= ImGuiTreeNodeFlags_Selected;

        //----- Leaf/Node -----//
        if(r)
        {
            if(r->getChildren().size() == 0)
                nodeFlags |= ImGuiTreeNodeFlags_Leaf;
        }
        else
            nodeFlags |= ImGuiTreeNodeFlags_Leaf;
        bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)i++, nodeFlags, name.c_str());

        //----- Select entity -----//
        if(ImGui::IsItemClicked())
            ComponentManager::setSelectedEntity(entity);

        //----- Popup -----//
        if(ImGui::BeginPopupContextItem())
        {
            if(ImGui::Selectable("Delete"))
                _entitiesToDelete.push_back(entity);

            if(ImGui::Selectable("Copy"))
                _entitiesToCopy.push_back(entity);
           ImGui::EndPopup();
        }

        //----- Drag/Drop entities -----//
        if(ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("EntityId", &entity, sizeof(EntityId));
            ImGui::Text(name.c_str());
            ImGui::EndDragDropSource();
        }
        if(ImGui::BeginDragDropTarget())
        {
            if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("EntityId"))
            {
                DASSERT(payload->DataSize == sizeof(EntityId), "Payload data should have same size as EntityId");
                EntityId childEntity = *(const EntityId*)payload->Data;
                RelationshipComponent::setParent(entity, childEntity);
            }
            ImGui::EndDragDropTarget();
        }

        //----- Render children -----//
        if(nodeOpen)
        {
            if(r && r->getChildren().size() > 0)
                for(auto child : r->getChildren())
                    renderTreeNode(child, i);
            ImGui::TreePop();
        }
    }

    void EntityWindow::renderComponents()
    {
        EntityId selected = ComponentManager::getSelectedEntity();
        if(selected == -1)
            return;

        ImGui::Text("Components (EntityId: %d)", selected);

        // Render options to edit each component
        for(auto compReg : ComponentManager::getComponentRegistries())
        {
            void* component = ComponentManager::getEntityComponentById(compReg->getId(), selected);
            if(component != nullptr)
            {
                std::string name = compReg->getDescription().type;
                if(compReg->getId() != TypedComponentRegistry<RelationshipComponent>::getInstance().getId())
                {
                    bool open = true;
                    if(ImGui::CollapsingHeader((name+"##Components"+name+"Header").c_str(), &open))
                        compReg->renderUI((Component*)component);
                    if(!open)
                        ComponentManager::removeEntityComponentById(compReg->getId(), selected);
                }
            }
        }

        // Align button
        const float size = 100.0f;
        ImVec2 cursor = ImGui::GetCursorPos();
        ImVec2 window = ImGui::GetWindowSize();
        ImGui::SetCursorPos(ImVec2((window.x - size)*0.5f, cursor.y));
        if(ImGui::Button("Add Component", ImVec2(size, 0)))
            ImGui::OpenPopup("Scene_ComponentAdd");


        if(ImGui::BeginPopup("Scene_ComponentAdd"))
        {
            for(auto compReg : ComponentManager::getComponentRegistries())
            {
                void* component = ComponentManager::getEntityComponentById(compReg->getId(), selected);
                if(component == nullptr)
                {
                    if(compReg->getId() != TypedComponentRegistry<RelationshipComponent>::getInstance().getId())
                    {
                        std::string type = compReg->getDescription().type;
                        if(ImGui::Selectable((type+"##ComponentAdd"+type).c_str()))
                            ComponentManager::addEntityComponentById(compReg->getId(), selected);
                    }
                }
            }
            ImGui::EndPopup();
        }
    }

    void EntityWindow::textureCombo(std::string comboId, StringId& sid)
    {
        std::vector<StringId> textures = ResourceManager::getResources<Texture>();

        if(sid == "Empty texture")
        {
            DASSERT(textures.size()>0, "At least one texture should be loaded to the memory");
            sid = textures[0];
        }


        if(ImGui::BeginCombo(("Texture##"+comboId).c_str(), fs::path(sid.getString()).filename().string().c_str()))
        {
            for(size_t i = 0; i < textures.size(); i++)
            {
                if(ImGui::Selectable(fs::path(textures[i].getString()).filename().string().c_str(), sid == textures[i]))
                    sid = textures[i];
                if(textures[i] == sid)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    void EntityWindow::renderCameraWindows()
    {
        for(auto eid : _cameraWindows)
        {
            CameraComponent* camera = ComponentManager::getEntityComponent<CameraComponent>(eid);
            bool open = true;
            ImGui::Begin(("Camera Entity "+std::to_string(eid)).c_str(), &open);
            ImVec2 size = ImVec2(camera->width, camera->height);
            ImGui::Image(SensorManager::getEntityCameraImGuiTexture(eid), size, ImVec2(0, 0), ImVec2(1, 1));
            ImGui::End();

            if(!open) _cameraWindows.erase(eid);
        }
    }
}
