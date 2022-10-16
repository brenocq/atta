//--------------------------------------------------
// Atta UI Module
// viewportWindows.cpp
// Date: 2021-12-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/material.h>
#include <atta/component/components/mesh.h>
#include <atta/component/components/name.h>
#include <atta/component/components/relationship.h>
#include <atta/component/components/rigidBody2D.h>
#include <atta/component/components/transform.h>
#include <atta/component/interface.h>
#include <atta/graphics/interface.h>
#include <atta/resource/interface.h>
#include <atta/resource/resources/material.h>
#include <atta/ui/layers/editor/windows/viewportWindows.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <ImGuizmo.h>

namespace atta::ui {

void ViewportWindows::render() {
    std::vector<std::shared_ptr<graphics::Viewport>> viewports = graphics::getViewports();
    static int activeViewport = 0;

    int i = -1;
    for (auto viewport : viewports) {
        // ImGui::DockBuilderDockWindow((viewport->getName()+"###Viewport"+viewport->getSID().getString()).c_str(), _dockSpace.getViewportDockId());
        char nameBuf[128];
        sprintf(nameBuf, "%s###Viewport%s", viewport->getName().c_str(), viewport->getSID().getString().c_str());
        i++;
        // Render and resize
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
        bool open = true;
        ImGui::Begin(nameBuf, &open);
        if (graphics::getViewportRendering()) {
            //----- Move camera -----//
            // Check started camera movement
            if (ImGui::IsMouseClicked(2) && ImGui::IsWindowHovered())
                activeViewport = i;

            // Update camera (wheel pressed)
            if (activeViewport == i && ImGui::IsWindowHovered()) {
                viewport->getCamera()->setViewportSize(viewport->getWidth(), viewport->getHeight());
                viewport->getCamera()->move();
            }

            //----- Mouse click -----//
            vec2i click = {-1, -1};
            if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()) {
                ImVec2 window = ImGui::GetWindowPos();
                ImVec2 cursor = ImGui::GetCursorPos();
                ImVec2 mouse = ImGui::GetMousePos();
                click = {int(mouse.x - window.x - cursor.x), int(mouse.y - window.y - cursor.y)};
            }

            if (ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered())
                ImGui::OpenPopup("Editor_AddBasicShape");
            addBasicShapePopup();

            //----- Keyboard click -----//
            static ImGuizmo::OPERATION mouseOperation = ImGuizmo::OPERATION::TRANSLATE;
            static ImGuizmo::MODE mouseMode = ImGuizmo::MODE::LOCAL;
            static bool snap = false;

            if (ImGui::IsWindowHovered()) {
                snap = false;
                ImGuiIO& io = ImGui::GetIO();
                if (ImGui::IsKeyPressed('T') && io.KeyCtrl) {
                    mouseOperation = ImGuizmo::OPERATION::TRANSLATE;
                    mouseMode = ImGuizmo::MODE::LOCAL;
                } else if (ImGui::IsKeyPressed('T') && io.KeyShift) {
                    mouseOperation = ImGuizmo::OPERATION::TRANSLATE;
                    mouseMode = ImGuizmo::MODE::WORLD;
                } else if (ImGui::IsKeyPressed('S') && io.KeyCtrl) {
                    mouseOperation = ImGuizmo::OPERATION::SCALE;
                    mouseMode = ImGuizmo::MODE::LOCAL;
                } else if (ImGui::IsKeyPressed('S') && io.KeyShift) {
                    mouseOperation = ImGuizmo::OPERATION::SCALE;
                    mouseMode = ImGuizmo::MODE::WORLD;
                } else if (ImGui::IsKeyPressed('R') && io.KeyCtrl) {
                    mouseOperation = ImGuizmo::OPERATION::ROTATE;
                    mouseMode = ImGuizmo::MODE::LOCAL;
                } else if (ImGui::IsKeyPressed('R') && io.KeyShift) {
                    mouseOperation = ImGuizmo::OPERATION::ROTATE;
                    mouseMode = ImGuizmo::MODE::WORLD;
                } else if (io.KeyCtrl)
                    snap = true;
            }

            //----- Render to texture -----//
            ImVec2 size = ImVec2(viewport->getWidth(), viewport->getHeight());
            ImGui::Image(viewport->getImGuiTexture(), size, ImVec2(0, 0), ImVec2(1, 1));

            //----- ImGuizmo -----//
            bool imGuizmoUsingMouse = false;
            component::EntityId entity = component::getSelectedEntity();
            if (entity >= 0) {
                component::Transform* t = component::getComponent<component::Transform>(entity);

                if (t) {
                    ImGuizmo::SetOrthographic(viewport->getCamera()->getName() == "OrthographicCamera");
                    ImGuizmo::SetDrawlist();
                    ImGuizmo::SetRect(ImGui::GetWindowPos().x + 5.0f, ImGui::GetWindowPos().y + 24.0f, viewport->getWidth(), viewport->getHeight());
                    mat4 view = transpose(viewport->getCamera()->getView());
                    mat4 proj = viewport->getCamera()->getProj();
                    proj.mat[1][1] *= -1;
                    proj.transpose();

                    mat4 transform = transpose(t->getWorldTransform(entity));

                    float snapValue = 0.5f;
                    if (mouseOperation == ImGuizmo::OPERATION::ROTATE)
                        snapValue = 45.0f;
                    float snapValues[3] = {snapValue, snapValue, snapValue};

                    ImGuizmo::Manipulate(view.data, proj.data, mouseOperation, mouseMode, transform.data, nullptr, snap ? snapValues : nullptr);

                    if (ImGuizmo::IsUsing()) {
                        imGuizmoUsingMouse = true;
                        transform.transpose();

                        // Get changed
                        vec3 pos, scale;
                        quat newOri;
                        transform.getPosOriScale(pos, newOri, scale);
                        vec3 oriDelta = newOri.getEuler() - t->orientation.getEuler();
                        quat ori;
                        ori.setEuler(t->orientation.getEuler() + oriDelta);

                        // Delta world to local
                        component::Relationship* r = component::getComponent<component::Relationship>(entity);
                        if (r && r->getParent() != -1) {
                            // Get transform of the first entity that has transform when going up in the hierarchy
                            component::Transform* pt = nullptr;
                            component::EntityId parentId = -1;
                            while (pt == nullptr) {
                                parentId = r->getParent();
                                pt = component::getComponent<component::Transform>(parentId);
                                r = component::getComponent<component::Relationship>(parentId);
                                if (r->getParent() == -1)
                                    break;
                            }

                            // If found some entity with transform component, convert result to be relative to it
                            if (pt) {
                                vec3 pPos, pScale;
                                quat pOri;
                                mat4 pTransform = pt->getWorldTransform(parentId);
                                pTransform.getPosOriScale(pPos, pOri, pScale);

                                // Calculate pos ori scale relative to parent
                                pos -= pPos;
                                scale /= pScale;
                                ori = ori * (-pOri); // Rotation from pOri to ori
                            }
                        }

                        // Update entity transform
                        if (mouseOperation == ImGuizmo::OPERATION::TRANSLATE)
                            t->position = pos;
                        else if (mouseOperation == ImGuizmo::OPERATION::ROTATE)
                            t->orientation = ori;
                        else if (mouseOperation == ImGuizmo::OPERATION::SCALE)
                            t->scale = scale;

                        component::RigidBody2D* rb2d = component::getComponent<component::RigidBody2D>(entity);
                        if (rb2d) {
                            if (mouseOperation == ImGuizmo::OPERATION::TRANSLATE || mouseOperation == ImGuizmo::OPERATION::ROTATE) {
                                vec2 pos = vec2(t->position);
                                float angle = -t->orientation.getEuler().z;
                                rb2d->setTransform(pos, angle);
                            } else if (mouseOperation == ImGuizmo::OPERATION::SCALE) {
                                // TODO Recreate box2d rigid body
                            }
                        }
                    }
                }
            }

            //----- Mouse click selection -----//
            if (!imGuizmoUsingMouse) {
                if (click.x >= 0 && click.y >= 0 && click.x < (int)viewport->getWidth() && click.y < (int)viewport->getHeight()) {
                    component::EntityId eid = graphics::viewportEntityClick(viewport, click);
                    component::setSelectedEntity(eid);
                }
            }

            //----- Overlay -----//
            //{
            //    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize |
            //                                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
            //                                    ImGuiWindowFlags_NoMove;
            //    ImVec2 window = ImGui::GetWindowPos();
            //    ImGui::SetNextWindowPos(ImVec2(window.x + 10, window.y + 30));
            //    ImGui::SetNextWindowBgAlpha(0.35f);
            //    bool open = true;
            //    if (ImGui::Begin((viewport->getSID().getString() + "Overlay").c_str(), &open, window_flags)) {
            //        ImGui::Text("To move the camera");
            //        ImGui::BulletText("Holding mouse middle button");
            //        ImGui::BulletText("Rotate with mouse");
            //        ImGui::BulletText("Move with ASWD QE");
            //        ImGui::Text("To move objects");
            //        ImGui::BulletText("Select some object");
            //        ImGui::BulletText("Translate: SHIFT+t");
            //        ImGui::BulletText("Scale: SHIFT+s");
            //        ImGui::BulletText("Rotate: SHIFT+r");
            //    }
            //    ImGui::End();
            //}

            //----- Resize -----//
            ImVec2 windowSize = ImGui::GetWindowSize();
            if (windowSize.x != size.x || windowSize.y != size.y)
                viewport->resize((uint32_t)windowSize.x - 10, (uint32_t)windowSize.y - 30);
        }
        ImGui::End();
        ImGui::PopStyleVar(1);

        if (!open)
            graphics::removeViewport(viewport);
    }
}

void ViewportWindows::addBasicShapePopup() {
    if (ImGui::BeginPopup("Editor_AddBasicShape")) {
        std::string basicShapes[] = {"Cube", "Sphere", "Cylinder", "Triangle", "Plane", "Disk"};
        StringId basicShapesMesh[] = {"meshes/cube.obj"_sid,     "meshes/sphere.obj"_sid, "meshes/cylinder.obj"_sid,
                                      "meshes/triangle.obj"_sid, "meshes/plane.obj"_sid,  "meshes/disk.obj"_sid};
        unsigned i = 0;
        for (auto shape : basicShapes) {
            if (ImGui::Selectable((shape + "##AddBasicShape" + shape).c_str())) {
                component::EntityId eid = component::createEntity();
                component::Name* n = component::addComponent<component::Name>(eid);
                strcpy(n->name, shape.c_str());
                component::addComponent<component::Transform>(eid);
                component::Mesh* m = component::addComponent<component::Mesh>(eid);
                m->sid = basicShapesMesh[i];
                resource::Material* matRes =
                    resource::create<resource::Material>("defaultMaterial." + std::to_string(eid), resource::Material::CreateInfo{});
                matRes->color = vec3(0.5f, 0.5f, 0.5f);
                component::Material* mat = component::addComponent<component::Material>(eid);
                mat->sid = matRes->getId();
                component::setSelectedEntity(eid);
            }
            i++;
        }
        ImGui::EndPopup();
    }
}

} // namespace atta::ui
