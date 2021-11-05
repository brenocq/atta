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
    EntityWindow::EntityWindow():
        _selected(0), _someSelected(false)
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
        if(ImGui::Button("Create"))
            ComponentManager::createEntity();

        std::vector<EntityId> entities = ComponentManager::getEntities();
        int i = 0;
        ImGui::Text("Scene");
        // Render root entities
        for(EntityId entity : entities)
        {
            RelationshipComponent* r = ComponentManager::getEntityComponent<RelationshipComponent>(entity);
            if(!r || (r && r->parent == -1))
                renderTreeNode(entity, i);
        }
    }

    void EntityWindow::renderTreeNode(EntityId entity, int& i)
    {
        //----- Selected -----//
        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        if(entity == _selected)
            nodeFlags |= ImGuiTreeNodeFlags_Selected;

        //----- Name -----//
        std::string name = "<Object "+std::to_string(entity)+">";
        NameComponent* n = ComponentManager::getEntityComponent<NameComponent>(entity);
        RelationshipComponent* r = ComponentManager::getEntityComponent<RelationshipComponent>(entity);
        if(n) name = n->name;

        //----- Leaf/Node -----//
        if(r)
        {
            if(r->children.size() == 0)
                nodeFlags |= ImGuiTreeNodeFlags_Leaf;
        }
        else
            nodeFlags |= ImGuiTreeNodeFlags_Leaf;
        bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)i++, nodeFlags, name.c_str());

        //----- Select -----//
        if(ImGui::IsItemClicked())
        {
            _selected = entity;
            _someSelected = true;
        }

        //----- Drag/Drop -----//
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
                LOG_DEBUG("EntityWindow", "Now $1 is child of $0", entity, childEntity);

                // Update new parent (entity that received drop)
                if(r)
                    r->children.push_back(childEntity);
                else
                {
                    // Create parent relationship
                    r = ComponentManager::addEntityComponent<RelationshipComponent>(entity);
                    r->parent = -1;
                    r->children = { childEntity };
                }

                // Update child relationship (entity that was dropped)
                RelationshipComponent* cr = ComponentManager::getEntityComponent<RelationshipComponent>(childEntity);
                if(cr)
                {
                    // Remove old entity parent if necessary
                    if(cr->parent != -1)
                    {
                        RelationshipComponent* oldp = ComponentManager::getEntityComponent<RelationshipComponent>(cr->parent);
                        DASSERT(oldp, "Entity dropped has parent, so parent must have parent relationship too");
                        for(size_t i = 0; i < oldp->children.size(); i++)
                            if(oldp->children[i] == childEntity)
                            {
                                oldp->children.erase(oldp->children.begin()+i);
                                break;
                            }
                    }
                }
                else
                {
                    // Create child relationship
                    cr = ComponentManager::addEntityComponent<RelationshipComponent>(childEntity);
                    cr->children = {};
                }
                // Set child parent
                cr->parent = entity;
            }
            ImGui::EndDragDropTarget();
        }

        //----- Children -----//
        if(nodeOpen)
        {
            if(r && r->children.size() > 0)
                for(auto child : r->children)
                    renderTreeNode(child, i);
            ImGui::TreePop();
        }
    }

    void EntityWindow::renderComponents()
    {
        if(!_someSelected)
            return;

        ImGui::Text("Components (ID:%d)", _selected);
        ImGui::SameLine();
        if(ImGui::Button("Add"))
            ImGui::OpenPopup("Scene_ComponentAdd");

        if(ImGui::BeginPopup("Scene_ComponentAdd"))
        {
            if(ImGui::Selectable("Name##ComponentAddName"))
                ComponentManager::addEntityComponent<NameComponent>(_selected);
            if(ImGui::Selectable("Transform##ComponentAddTransform"))
                ComponentManager::addEntityComponent<TransformComponent>(_selected);
            if(ImGui::Selectable("Mesh##ComponentAddMesh"))
                ComponentManager::addEntityComponent<MeshComponent>(_selected);
            if(ImGui::Selectable("Material##ComponentAddMaterial"))
                ComponentManager::addEntityComponent<MaterialComponent>(_selected);

            ImGui::Separator();
            if(ImGui::Selectable("Point Light##ComponentAddPointLight"))
                ComponentManager::addEntityComponent<PointLightComponent>(_selected);
            if(ImGui::Selectable("Directional Light##ComponentAddDirectionalLight"))
                ComponentManager::addEntityComponent<DirectionalLightComponent>(_selected);

            ImGui::Separator();

            if(ImGui::Selectable("Script##ComponentAddScript"))
            {
                ScriptComponent* script = ComponentManager::addEntityComponent<ScriptComponent>(_selected);
                std::vector<StringId> scriptSids = ScriptManager::getScriptSids();
                if(scriptSids.size())
                    script->sid = scriptSids[0];
            }

            if(ImGui::Selectable("Prototype##ComponentAddPrototype"))
            {
                PrototypeComponent* prototype = ComponentManager::addEntityComponent<PrototypeComponent>(_selected);
                prototype->maxClones = 1;
            }

            ImGui::Separator();

            if(ImGui::Selectable("Camera##ComponentAddCamera"))
            {
                CameraComponent* camera = ComponentManager::addEntityComponent<CameraComponent>(_selected);
                *camera = CameraComponent{};
            }

            ImGui::EndPopup();
        }

        // Render options to edit each component
        for(auto compReg : ComponentManager::getComponentRegistries())
        {
            void* component = ComponentManager::getEntityComponentById(compReg->getId(), _selected);
            if(component != nullptr)
            {
                std::string name = compReg->getDescription().type;
                if(ImGui::CollapsingHeader((name+"##Components"+name+"Header").c_str(), ImGuiTreeNodeFlags_None))
                    compReg->renderUI((Component*)component);
            }
        }
    }

    bool EntityWindow::getSelected(EntityId& eid)
    {
        eid = _selected;
        return _someSelected;
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
