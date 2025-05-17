//--------------------------------------------------
// Atta UI Module
// viewportWindows.cpp
// Date: 2021-12-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/base.h>
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
#include <atta/ui/interface.h>
#include <atta/ui/windows/viewport/viewportWindows.h>

#include <imgui.h>
#include <imgui_internal.h>

namespace atta::ui {

void ViewportWindows::startUp() {
    _computeEntityClick = std::make_unique<gfx::EntityClick>();
    createDefaultViewports();
}

void ViewportWindows::shutDown() {
    _computeEntityClick.reset();
    _openModals.clear();
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
    _viewportsNext = _viewports;

    if (_viewportRendering)
        for (auto& viewport : _viewports)
            viewport->render();
}

void ViewportWindows::renderUI() {
    PROFILE();
    static int activeViewport = 0;

    // Viewports FPS
    static clock_t vpLastTime = std::clock();
    const clock_t vpCurrTime = std::clock();
    const float vpTimeDiff = float(vpCurrTime - vpLastTime) / CLOCKS_PER_SEC;
    if (_viewportRendering && (_viewportFPS > 0 && (vpTimeDiff > 1 / _viewportFPS))) {
        vpLastTime = vpCurrTime;
    }

    int i = -1;
    for (auto viewport : _viewports) {
        char nameBuf[128];
        sprintf(nameBuf, "%s##AttaViewport", viewport->getName().c_str());
        i++;

        // Docking
        ImGui::SetNextWindowDockID(ui::getViewportDockId(), ImGuiCond_FirstUseEver);

        // Render and resize
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        bool open = true;
        if (ImGui::Begin(nameBuf, &open)) {
            // Viewport context menu
            if (ImGui::BeginPopupContextItem("ViewportWindowContextMenu")) {
                if (ImGui::MenuItem("Edit"))
                    _openModals[viewport->getSID()] = true;
                ImGui::EndPopup();
            }

            if (_viewportRendering) {
                //----- Move camera -----//
                // Check started camera movement
                if (ImGui::IsMouseClicked(2) && ImGui::IsWindowHovered())
                    activeViewport = i;

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

                //----- Operation selection -----//
                if (ImGui::IsWindowHovered() && !ImGui::IsMouseDown(ImGuiMouseButton_Middle)) {
                    ImGuiIO& io = ImGui::GetIO();

                    if (io.KeyCtrl)
                        _gizmo.setMode(Gizmo::LOCAL);
                    if (io.KeyShift)
                        _gizmo.setMode(Gizmo::WORLD);

                    if (ImGui::IsKeyPressed(ImGuiKey_T))
                        _gizmo.setOperation(Gizmo::TRANSLATE);
                    else if (ImGui::IsKeyPressed(ImGuiKey_S))
                        _gizmo.setOperation(Gizmo::SCALE);
                    else if (ImGui::IsKeyPressed(ImGuiKey_R))
                        _gizmo.setOperation(Gizmo::ROTATE);
                }

                //----- Render viewport to texture -----//
                ImVec2 size = ImVec2(viewport->getWidth(), viewport->getHeight());
                ImGui::Image((ImTextureID)(intptr_t)viewport->getImGuiTexture(), size, ImVec2(0, 0), ImVec2(1, 1));

                //----- Gizmo manipulation -----//
                bool gizmoUsingMouse = false;
                _gizmo.setViewport(viewport);
                cmp::EntityId eid = component::getSelectedEntity();
                if (eid >= 0 && _gizmo.manipulate(eid))
                    gizmoUsingMouse = true;

                //----- Camera control -----//
                if (activeViewport == i && ImGui::IsWindowHovered()) {
                    viewport->getCamera()->setViewportSize(viewport->getWidth(), viewport->getHeight());
                    viewport->getCamera()->move();
                }

                //----- Mouse click selection -----//
                if (!gizmoUsingMouse) {
                    if (click.x >= 0 && click.y >= 0 && click.x < (int)viewport->getWidth() && click.y < (int)viewport->getHeight()) {
                        cmp::EntityId eid = _computeEntityClick->click(viewport->getRenderer(), viewport->getCamera(), click);
                        cmp::setSelectedEntity(eid);
                    }
                }

                //----- Resize -----//
                ImVec2 windowSize = ImGui::GetWindowSize();
                if (windowSize.x != size.x || windowSize.y != size.y)
                    viewport->resize((uint32_t)windowSize.x - 10, (uint32_t)windowSize.y - 30);
            }
        }
        ImGui::End();
        ImGui::PopStyleVar(1);

        if (!open)
            removeViewport(viewport);
    }

    renderModals();
}

const std::vector<std::shared_ptr<Viewport>>& ViewportWindows::getViewports() const { return _viewports; }

void ViewportWindows::clearViewports() {
    _openModals.clear();
    _viewportsNext.clear();
    _swapViewports = true;
}

void ViewportWindows::addViewport(std::shared_ptr<Viewport> viewport) {
    _viewportsNext.push_back(viewport);
    _swapViewports = true;
}

void ViewportWindows::removeViewport(std::shared_ptr<Viewport> viewport) {
    _openModals[viewport->getSID()] = false;
    for (unsigned i = 0; i < _viewportsNext.size(); i++)
        if (_viewportsNext[i] == viewport) {
            _viewportsNext.erase(_viewportsNext.begin() + i);
            break;
        }
    _swapViewports = true;
}

void ViewportWindows::openViewportModal(StringId sid) { _openModals[sid] = true; }

void ViewportWindows::renderModals() {
    for (auto& viewport : _viewports) {
        char nameBuf[196];
        sprintf(nameBuf, "%s###ViewportModal%s", viewport->getName().c_str(), viewport->getSID().getString().c_str());

        StringId sid = viewport->getSID();
        bool open = _openModals[sid];
        if (open) {
            ImGui::SetNextWindowSize(ImVec2(350, 250), ImGuiCond_FirstUseEver);
            if (ImGui::Begin(nameBuf, &open)) {
                viewport->renderUI();

                ImGui::Separator();
                if (ImGui::Button("Delete Viewport")) {
                    removeViewport(viewport);
                    ImGui::End();
                    break;
                }
            }
            ImGui::End();
            _openModals[sid] = open;
        }
    }
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
