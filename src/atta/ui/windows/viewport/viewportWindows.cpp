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
#include <atta/graphics/cameras/perspectiveCamera.h>
#include <atta/graphics/interface.h>
#include <atta/graphics/renderers/pbrRenderer.h>
#include <atta/resource/interface.h>
#include <atta/resource/resources/material.h>
#include <atta/ui/windows/viewport/viewportWindows.h>
#include <atta/ui/interface.h>

#include <imgui.h>
#include <imgui_internal.h>

namespace atta::ui {

void ViewportWindows::startUp() {
    _computeEntityClick = std::make_unique<gfx::EntityClick>();
    createDefaultViewports();
}

void ViewportWindows::shutDown() {
    _computeEntityClick.reset();
    _viewports.clear();
    _viewportsNext.clear();
}

void ViewportWindows::renderViewports() {
    PROFILE();
    if (_swapViewports) {
        _viewports = _viewportsNext;
        _viewportsNext.clear();
        _swapViewports = false;
    }

    if (_viewportRendering)
        for (auto& viewport : _viewports)
            viewport->render();
}

void ViewportWindows::renderUI() {
    PROFILE();
    static int activeViewport = 0;

    // Viewports fps
    static clock_t vpLastTime = std::clock();
    const clock_t vpCurrTime = std::clock();
    const float vpTimeDiff = float(vpCurrTime - vpLastTime) / CLOCKS_PER_SEC;
    if (_viewportRendering && (_viewportFPS > 0 && (vpTimeDiff > 1 / _viewportFPS))) {
        vpLastTime = vpCurrTime;
    }

    int i = -1;
    for (auto viewport : _viewports) {
        // TODO docking
        // ImGui::DockBuilderDockWindow((viewport->getName()+"###Viewport"+viewport->getSID().getString()).c_str(),
        //_dockSpace.getViewportDockId());
        char nameBuf[128];
        sprintf(nameBuf, "%s###Viewport%s", viewport->getName().c_str(), viewport->getSID().getString().c_str());
        i++;

        // Docking
        ImGui::SetNextWindowDockID(ui::getViewportDockId(), ImGuiCond_FirstUseEver);

        // Render and resize
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        bool open = true;
        ImGui::Begin(nameBuf, &open);
        if (_viewportRendering) {
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
            // static ImGuizmo::OPERATION mouseOperation = ImGuizmo::OPERATION::TRANSLATE;
            // static ImGuizmo::MODE mouseMode = ImGuizmo::MODE::LOCAL;
            // static bool snap = false;

            // if (ImGui::IsWindowHovered()) {
            //     snap = false;
            //     ImGuiIO& io = ImGui::GetIO();
            //     if (ImGui::IsKeyPressed(ImGuiKey_T) && io.KeyCtrl) {
            //         mouseOperation = ImGuizmo::OPERATION::TRANSLATE;
            //         mouseMode = ImGuizmo::MODE::LOCAL;
            //     } else if (ImGui::IsKeyPressed(ImGuiKey_T) && io.KeyShift) {
            //         mouseOperation = ImGuizmo::OPERATION::TRANSLATE;
            //         mouseMode = ImGuizmo::MODE::WORLD;
            //     } else if (ImGui::IsKeyPressed(ImGuiKey_S) && io.KeyCtrl) {
            //         mouseOperation = ImGuizmo::OPERATION::SCALE;
            //         mouseMode = ImGuizmo::MODE::LOCAL;
            //     } else if (ImGui::IsKeyPressed(ImGuiKey_S) && io.KeyShift) {
            //         mouseOperation = ImGuizmo::OPERATION::SCALE;
            //         mouseMode = ImGuizmo::MODE::WORLD;
            //     } else if (ImGui::IsKeyPressed(ImGuiKey_R) && io.KeyCtrl) {
            //         mouseOperation = ImGuizmo::OPERATION::ROTATE;
            //         mouseMode = ImGuizmo::MODE::LOCAL;
            //     } else if (ImGui::IsKeyPressed(ImGuiKey_R) && io.KeyShift) {
            //         mouseOperation = ImGuizmo::OPERATION::ROTATE;
            //         mouseMode = ImGuizmo::MODE::WORLD;
            //     } else if (io.KeyCtrl)
            //         snap = true;
            // }

            //----- Render to texture -----//
            ImVec2 size = ImVec2(viewport->getWidth(), viewport->getHeight());
            ImGui::Image((ImTextureID)(intptr_t)viewport->getImGuiTexture(), size, ImVec2(0, 0), ImVec2(1, 1));

            //    //----- ImGuizmo -----//
            //    bool imGuizmoUsingMouse = false;
            //    component::EntityId entity = component::getSelectedEntity();
            //    if (entity >= 0) {
            //        component::Transform* t = component::getComponent<component::Transform>(entity);

            //        if (t) {
            //            ImGuizmo::SetOrthographic(viewport->getCamera()->getName() == "OrthographicCamera");
            //            ImGuizmo::SetDrawlist();
            //            ImGuizmo::SetRect(ImGui::GetWindowPos().x + 5.0f, ImGui::GetWindowPos().y + 24.0f, viewport->getWidth(),
            //            viewport->getHeight());
            //            mat4 view = transpose(viewport->getCamera()->getView());
            //            mat4 proj = viewport->getCamera()->getProj();
            //            proj.mat[1][1] *= -1;
            //            proj.transpose();

            //            mat4 transform = transpose(t->getWorldTransformMatrix(entity));

            //            float snapValue = 0.5f;
            //            if (mouseOperation == ImGuizmo::OPERATION::ROTATE)
            //                snapValue = 45.0f;
            //            float snapValues[3] = {snapValue, snapValue, snapValue};

            //            ImGuizmo::Manipulate(view.data, proj.data, mouseOperation, mouseMode, transform.data, nullptr, snap ? snapValues : nullptr);

            //            if (ImGuizmo::IsUsing()) {
            //                imGuizmoUsingMouse = true;
            //                transform.transpose();

            //                // Get changed
            //                vec3 pos, scale;
            //                quat newOri;
            //                transform.getPosOriScale(pos, newOri, scale);
            //                vec3 oriDelta = newOri.getEuler() - t->orientation.getEuler();
            //                quat ori;
            //                ori.setEuler(t->orientation.getEuler() + oriDelta);

            //                // Delta world to local
            //                component::Relationship* r = component::getComponent<component::Relationship>(entity);
            //                if (r && r->getParent() != -1) {
            //                    // Get transform of the first entity that has transform when going up in the hierarchy
            //                    component::Transform* pt = nullptr;
            //                    component::EntityId parentId = -1;
            //                    while (pt == nullptr) {
            //                        parentId = r->getParent();
            //                        pt = component::getComponent<component::Transform>(parentId);
            //                        r = component::getComponent<component::Relationship>(parentId);
            //                        if (r->getParent() == -1)
            //                            break;
            //                    }

            //                    // If found some entity with transform component, convert result to be relative to it
            //                    if (pt) {
            //                        component::Transform pTransform = pt->getWorldTransform(parentId);
            //                        vec3 pPos = pTransform.position;
            //                        vec3 pScale = pTransform.scale;
            //                        quat pOri = pTransform.orientation;

            //                        // Calculate pos ori scale relative to parent
            //                        pos -= pPos;
            //                        scale /= pScale;
            //                        ori = ori * (-pOri); // Rotation from pOri to ori
            //                    }
            //                }

            //                // Update entity transform
            //                if (mouseOperation == ImGuizmo::OPERATION::TRANSLATE)
            //                    t->position = pos;
            //                else if (mouseOperation == ImGuizmo::OPERATION::ROTATE)
            //                    t->orientation = ori;
            //                else if (mouseOperation == ImGuizmo::OPERATION::SCALE)
            //                    t->scale = scale;

            //                // component::RigidBody2D* rb2d = component::getComponent<component::RigidBody2D>(entity);
            //                // if (rb2d) {
            //                //     if (mouseOperation == ImGuizmo::OPERATION::TRANSLATE || mouseOperation == ImGuizmo::OPERATION::ROTATE) {
            //                //         vec2 pos = vec2(t->position);
            //                //         float angle = -t->orientation.getEuler().z;
            //                //         rb2d->setTransform(pos, angle);
            //                //     } else if (mouseOperation == ImGuizmo::OPERATION::SCALE) {
            //                //         // TODO Recreate box2d rigid body
            //                //     }
            //                // }
            //            }
            //        }
            //    }

            //    //----- Mouse click selection -----//
            //    if (!imGuizmoUsingMouse) {
            //        if (click.x >= 0 && click.y >= 0 && click.x < (int)viewport->getWidth() && click.y < (int)viewport->getHeight()) {
            //            component::EntityId eid = _computeEntityClick(viewport->getRenderer(), viewport->getCamera(), click);
            //            component::setSelectedEntity(eid);
            //        }
            //    }

            //----- Resize -----//
            ImVec2 windowSize = ImGui::GetWindowSize();
            if (windowSize.x != size.x || windowSize.y != size.y)
                viewport->resize((uint32_t)windowSize.x - 10, (uint32_t)windowSize.y - 30);
        }
        ImGui::End();
        ImGui::PopStyleVar(1);

        if (!open)
            removeViewport(viewport);
    }
}

const std::vector<std::shared_ptr<Viewport>>& ViewportWindows::getViewports() const { return _viewports; }

void ViewportWindows::clearViewports() {
    _viewportsNext.clear();
    _swapViewports = true;
}

void ViewportWindows::addViewport(std::shared_ptr<Viewport> viewport) {
    _viewportsNext = _viewports;
    _viewportsNext.push_back(viewport);
    _swapViewports = true;
}

void ViewportWindows::removeViewport(std::shared_ptr<Viewport> viewport) {
    _viewportsNext = _viewports;
    for (unsigned i = 0; i < _viewportsNext.size(); i++)
        if (_viewportsNext[i] == viewport) {
            _viewportsNext.erase(_viewportsNext.begin() + i);
            break;
        }
    _swapViewports = true;
}

void ViewportWindows::openViewportModal(StringId sid) const {
    // XXX
    // std::vector<std::shared_ptr<graphics::Viewport>> viewports = graphics::getViewports();
    // static std::vector<bool> newViewportModals; // If first time creating the modal
    //_viewportModals.resize(viewports.size());

    //// Check if first time creating viewport
    // newViewportModals.resize(_viewportModals.size());
    // for (unsigned i = 0; i < newViewportModals.size(); i++)
    //     newViewportModals[i] = !newViewportModals[i] && _viewportModals[i];

    // for (uint32_t i = 0; i < _viewportModals.size(); i++) {
    //     char nameBuf[196];
    //     sprintf(nameBuf, "%s###ViewportProps%s", viewports[i]->getName().c_str(), viewports[i]->getSID().getString().c_str());

    //    bool open = _viewportModals[i];
    //    if (open) {
    //        if (newViewportModals[i])
    //            ImGui::SetNextWindowSize(ImVec2(200.0f, 300.0f));
    //        if (ImGui::Begin(nameBuf, &open)) {
    //            viewports[i]->renderUI();

    //            ImGui::Separator();
    //            if (ImGui::Button("Delete Viewport")) {
    //                graphics::removeViewport(viewports[i]);
    //                ImGui::End();
    //                break;
    //            }
    //        }
    //        ImGui::End();
    //        _viewportModals[i] = open;
    //    }
    //}
    // newViewportModals = _viewportModals;
}

void ViewportWindows::createDefaultViewports() {
    _viewportsNext.clear();

    Viewport::CreateInfo viewportInfo;
    viewportInfo.renderer = std::make_shared<gfx::PbrRenderer>();
    viewportInfo.camera = std::make_shared<gfx::PerspectiveCamera>(gfx::PerspectiveCamera::CreateInfo{});
    viewportInfo.sid = StringId("Main Viewport");
    _viewportsNext.push_back(std::make_shared<Viewport>(viewportInfo));
    _swapViewports = true;
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

                resource::Material::CreateInfo info{};
                info.color = vec3(0.5f, 0.5f, 0.5f);
                resource::Material* matRes = resource::create<resource::Material>("defaultMaterial." + std::to_string(eid), info);
                component::Material* mat = component::addComponent<component::Material>(eid);
                mat->sid = matRes->getId();
                component::setSelectedEntity(eid);
            }
            i++;
        }
        ImGui::EndPopup();
    }
}

bool ViewportWindows::getViewportRendering() const { return _viewportRendering; }
void ViewportWindows::setViewportRendering(bool viewportRendering) { _viewportRendering = viewportRendering; }

} // namespace atta::ui
