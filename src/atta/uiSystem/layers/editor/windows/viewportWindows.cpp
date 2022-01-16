//--------------------------------------------------
// Atta UI System
// viewportWindows.cpp
// Date: 2021-12-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/uiSystem/layers/editor/windows/viewportWindows.h>
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/components/transformComponent.h>
#include <atta/componentSystem/components/relationshipComponent.h>
#include <atta/componentSystem/components/nameComponent.h>
#include <atta/componentSystem/components/meshComponent.h>
#include <atta/componentSystem/components/materialComponent.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

namespace atta::ui
{
    void ViewportWindows::render()
    {
        std::vector<std::shared_ptr<Viewport>> viewports = GraphicsManager::getViewports();
        static int activeViewport = 0;

        int i = -1;
        for(auto viewport : viewports)
        {
            //ImGui::DockBuilderDockWindow((viewport->getName()+"###Viewport"+viewport->getSID().getString()).c_str(), _dockSpace.getViewportDockId());
            char nameBuf[128];
            sprintf(nameBuf, "%s###Viewport%s", viewport->getName().c_str(), viewport->getSID().getString().c_str());
            i++;
            // Render and resize
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
            bool open = true;
            ImGui::Begin(nameBuf, &open);
            {
                //----- Move camera -----//
                // Check started camera movement
                if(ImGui::IsMouseClicked(2) && ImGui::IsWindowHovered())
                    activeViewport = i;

                // Update camera (wheel pressed)
                if(activeViewport==i && ImGui::IsWindowHovered())
                {
                    viewport->getCamera()->setViewportSize(viewport->getWidth(), viewport->getHeight());
                    viewport->getCamera()->move();
                }

                //----- Mouse click -----//
                vec2i click = {-1, -1};
                if(ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
                {
                    ImVec2 window = ImGui::GetWindowPos();
                    ImVec2 cursor = ImGui::GetCursorPos();
                    ImVec2 mouse = ImGui::GetMousePos();
                    click = { int(mouse.x-window.x-cursor.x), int(mouse.y-window.y-cursor.y) };
                }

                if(ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered())
                    ImGui::OpenPopup("Editor_AddBasicShape");
                addBasicShapePopup();

                //----- Keyboard click -----//
                static ImGuizmo::OPERATION mouseOperation = ImGuizmo::OPERATION::TRANSLATE;
                static ImGuizmo::MODE mouseMode = ImGuizmo::MODE::LOCAL;
                static bool snap = false;

                if(ImGui::IsWindowHovered())
                {
                    snap = false;
                    ImGuiIO& io = ImGui::GetIO();
                    if(ImGui::IsKeyPressed('T') && io.KeyCtrl)
                    {
                        mouseOperation = ImGuizmo::OPERATION::TRANSLATE;
                        mouseMode = ImGuizmo::MODE::LOCAL;
                    }
                    else if(ImGui::IsKeyPressed('T') && io.KeyShift)
                    {
                        mouseOperation = ImGuizmo::OPERATION::TRANSLATE;
                        mouseMode = ImGuizmo::MODE::WORLD;
                    }
                    else if(ImGui::IsKeyPressed('S') && io.KeyCtrl)
                    {
                        mouseOperation = ImGuizmo::OPERATION::SCALE;
                        mouseMode = ImGuizmo::MODE::LOCAL;
                    }
                    else if(ImGui::IsKeyPressed('S') && io.KeyShift)
                    {
                        mouseOperation = ImGuizmo::OPERATION::SCALE;
                        mouseMode = ImGuizmo::MODE::WORLD;
                    }
                    else if(ImGui::IsKeyPressed('R') && io.KeyCtrl)
                    {
                        mouseOperation = ImGuizmo::OPERATION::ROTATE;
                        mouseMode = ImGuizmo::MODE::LOCAL;
                    }
                    else if(ImGui::IsKeyPressed('R') && io.KeyShift)
                    {
                        mouseOperation = ImGuizmo::OPERATION::ROTATE;
                        mouseMode = ImGuizmo::MODE::WORLD;
                    }
                    else if(io.KeyCtrl)
                        snap = true;
                }

                //----- Render to texture -----//
                ImVec2 size = ImVec2(viewport->getWidth(), viewport->getHeight());
                ImGui::Image(viewport->getImGuiTexture(), size, ImVec2(0, 0), ImVec2(1, 1));

                //----- ImGuizmo -----//
                bool imGuizmoUsingMouse = false;
                EntityId entity = ComponentManager::getSelectedEntity();
                if(entity >= 0 && ImGui::IsWindowHovered())
                {
                    TransformComponent* t = ComponentManager::getEntityComponent<TransformComponent>(entity);

                    if(t)
                    {
                        ImGuizmo::SetOrthographic(viewport->getCamera()->getName() == "OrthographicCamera");
                        ImGuizmo::SetDrawlist();
                        ImGuizmo::SetRect(ImGui::GetWindowPos().x+5.0f, ImGui::GetWindowPos().y+24.0f, viewport->getWidth(), viewport->getHeight());
                        mat4 view = transpose(viewport->getCamera()->getView());
                        mat4 proj = viewport->getCamera()->getProj();
                        proj.mat[1][1] *= -1;
                        proj.transpose();

                        mat4 transform = transpose(t->getWorldTransform(entity));

                        float snapValue = 0.5f;
                        if(mouseOperation == ImGuizmo::OPERATION::ROTATE)
                            snapValue = 45.0f;
                        float snapValues[3] = { snapValue, snapValue, snapValue };

                        ImGuizmo::Manipulate(view.data, proj.data, mouseOperation, mouseMode, transform.data, nullptr, snap ? snapValues : nullptr);

                        if(ImGuizmo::IsUsing())
                        {
                            imGuizmoUsingMouse = true;
                            transform.transpose();

                            // Delta world to local
                            RelationshipComponent* r = ComponentManager::getEntityComponent<RelationshipComponent>(entity);
                            if(r && r->getParent() != -1)
                            {
                                // Get transform of the first entity that has transform when going up in the hierarchy
                                TransformComponent* pt = nullptr;
                                EntityId parentId = -1;
                                while(pt == nullptr)
                                {
                                    parentId = r->getParent();
                                    pt = ComponentManager::getEntityComponent<TransformComponent>(parentId);
                                    r = ComponentManager::getEntityComponent<RelationshipComponent>(parentId);
                                    if(r->getParent() == -1)
                                        break;
                                }

                                // If found some entity with transform component, convert world transform to local
                                // newM1 = M2^(-1)*M3^(-1)*M3*M2*M1
                                // newM1 = (M3*M2)^(-1)*M3*M2*M1
                                if(pt) transform = inverse(pt->getWorldTransform(parentId)) * transform;
                            }

                            // Update entity transform
                            transform.getPosOriScale(t->position, t->orientation, t->scale);
                        }
                    }
                }

                //----- Mouse click selection -----//
                if(!imGuizmoUsingMouse)
                {
                    if(click.x >= 0 && click.y >=0 && click.x < (int)viewport->getWidth() && click.y < (int)viewport->getHeight())
                    {
                        EntityId eid = GraphicsManager::viewportEntityClick(viewport, click);
                        ComponentManager::setSelectedEntity(eid);
                    }
                }

                //----- Overlay -----//
                {
                    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
                    ImVec2 window = ImGui::GetWindowPos();
                    ImGui::SetNextWindowPos(ImVec2(window.x+10, window.y+30));
                    ImGui::SetNextWindowBgAlpha(0.35f);
                    bool open = true;
                    if(ImGui::Begin((viewport->getSID().getString()+"Overlay").c_str(), &open, window_flags))
                    {
                        ImGui::Text("To move the camera");
                        ImGui::BulletText("Holding mouse middle button");
                        ImGui::BulletText("Rotate with mouse");
                        ImGui::BulletText("Move with ASWD QE");
                        ImGui::Text("To move objects");
                        ImGui::BulletText("Select some object");
                        ImGui::BulletText("Translate: SHIFT+t");
                        ImGui::BulletText("Scale: SHIFT+s");
                        ImGui::BulletText("Rotate: SHIFT+r");
                    }
                    ImGui::End();
                }

                //----- Resize -----//
                ImVec2 windowSize = ImGui::GetWindowSize();
                if(windowSize.x != size.x || windowSize.y != size.y)
                    viewport->resize((uint32_t)windowSize.x-10, (uint32_t)windowSize.y-30);
            }
            ImGui::End();
            ImGui::PopStyleVar(1);

            if(!open)
                GraphicsManager::removeViewport(viewport);
        }
    }

    void ViewportWindows::addBasicShapePopup()
    {
        if(ImGui::BeginPopup("Editor_AddBasicShape"))
        {
            std::string basicShapes[] = { "Cube", "Sphere", "Cylinder", "Triangle", "Plane", "Disk" };
            StringId basicShapesMesh[] = { "meshes/cube.obj"_sid, "meshes/sphere.obj"_sid, "meshes/cylinder.obj"_sid, "meshes/triangle.obj"_sid, "meshes/plane.obj"_sid, "meshes/disk.obj"_sid };
            unsigned i = 0; 
            for(auto shape : basicShapes)
            {
                if(ImGui::Selectable((shape+"##AddBasicShape"+shape).c_str()))
                {
                    EntityId eid = ComponentManager::createEntity();
                    NameComponent* n = ComponentManager::addEntityComponent<NameComponent>(eid);
                    strcpy(n->name, shape.c_str());
                    ComponentManager::addEntityComponent<TransformComponent>(eid);
                    MeshComponent* m = ComponentManager::addEntityComponent<MeshComponent>(eid);
                    m->sid = basicShapesMesh[i];
                    MaterialComponent* mat = ComponentManager::addEntityComponent<MaterialComponent>(eid);
                    mat->albedo = vec3(0.5f, 0.5f, 0.5f);
                    ComponentManager::setSelectedEntity(eid);
                }
                i++;
            }
            ImGui::EndPopup();
        }
    }

}