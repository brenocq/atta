//--------------------------------------------------
// Atta UI Module
// editor.cpp
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/interface.h>
#include <atta/resource/interface.h>
#include <atta/script/interface.h>
#include <atta/ui/editor.h>
#include <imgui_internal.h>
#include <implot.h>
#include <implot3d.h>

#include <atta/ui/panels/topBar/localWindows/versionWindow.h>
#include <atta/ui/windows/graphicsModuleWindow.h>
#include <atta/ui/windows/ioModuleWindow.h>
#include <atta/ui/windows/physicsModuleWindow.h>
#include <atta/ui/windows/sensorModuleWindow.h>
#include <atta/ui/windows/timeProfiler/timeProfilerWindow.h>
#include <atta/ui/windows/utils/fileSelectionWindow.h>

#include <atta/component/components/name.h>
#include <atta/component/interface.h>
#include <atta/sensor/manager.h>

namespace atta::ui {

void Editor::startUp() { _viewportWindows.startUp(); }

void Editor::shutDown() { _viewportWindows.shutDown(); }

void Editor::renderViewports() { _viewportWindows.renderViewports(); }

void Editor::render() {
    PROFILE();
    setupDocking();

    // Demo
    ImGui::ShowDemoWindow();
    // ImPlot::ShowDemoWindow();
    // ImPlot3D::ShowDemoWindow();

    // Top interface
    _topBar.render();
    _toolBar.render();

    // Drawers
    _physicsDrawer.update();
    _sensorDrawer.update();

    // Windows
    _entityWindow.render();
    _logWindow.render();
    _viewportWindows.renderUI();

    // Module windows
    GraphicsModuleWindow::render();
    IOModuleWindow::render();
    PhysicsModuleWindow::render();
    SensorModuleWindow::render();

    // Tools
    TimeProfilerWindow::render();

    // Windows utils
    FileSelectionWindow::render();

    // Topbar windows
    VersionWindow::render();

    renderCameraWindows();

    // Project UI
    script::ProjectScript* project = script::getProjectScript();
    if (project)
        try {
            project->onUIRender();
        } catch (std::exception& e) {
            LOG_ERROR("ui::Editor", "Error when executing [w]ProjectScript::onUIRender[]: $0", e.what());
        }
}

const std::vector<std::shared_ptr<ui::Viewport>>& Editor::getViewports() const { return _viewportWindows.getViewports(); }
void Editor::openViewportModal(StringId sid) { _viewportWindows.openViewportModal(sid); }
void Editor::clearViewports() { _viewportWindows.clearViewports(); }
void Editor::addViewport(std::shared_ptr<ui::Viewport> viewport) { _viewportWindows.addViewport(viewport); }
bool Editor::getViewportRendering() const { return _viewportWindows.getViewportRendering(); }
void Editor::setViewportRendering(bool viewportRendering) { _viewportWindows.setViewportRendering(viewportRendering); }

void Editor::setupDocking() {
    //----- Create DockSpace -----//
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    _viewportDockId = ImGui::GetID("##AttaDockSpace");
    ImGui::DockSpaceOverViewport(_viewportDockId, viewport, ImGuiDockNodeFlags_NoWindowMenuButton);

    //----- Create DockSpace nodes -----//
    static bool firstRender = true;
    if (firstRender) {
        firstRender = false;

        // Create main docking nodes
        ImGuiID dockIdLeft, dockIdRight, dockIdUp, dockIdDown, dockIdCenter;
        ImGui::DockBuilderSplitNode(_viewportDockId, ImGuiDir_Left, 0.2f, &dockIdLeft, &dockIdCenter);
        ImGui::DockBuilderSplitNode(dockIdCenter, ImGuiDir_Right, 0.2f, &dockIdRight, &dockIdCenter);
        ImGui::DockBuilderSplitNode(dockIdCenter, ImGuiDir_Down, 0.2f, &dockIdDown, &dockIdCenter);
        ImGui::DockBuilderSplitNode(dockIdCenter, ImGuiDir_Up, 0.0f, &dockIdUp, &dockIdCenter);

        // Disable split for toolbar node
        ImGuiDockNode* node = ImGui::DockBuilderGetNode(dockIdUp);
        if (node)
            node->LocalFlags |= ImGuiDockNodeFlags_NoSplit | ImGuiDockNodeFlags_NoDockingOverMe;

        // Default docking for viewport windows
        ImGui::DockBuilderDockWindow("###AttaViewport::Main Viewport", dockIdCenter);

        // Default docking for module windows
        ImGui::DockBuilderDockWindow("IO Module##Atta", dockIdLeft);
        ImGui::DockBuilderDockWindow("Graphics Module##Atta", dockIdLeft);
        ImGui::DockBuilderDockWindow("Physics Module##Atta", dockIdLeft);
        ImGui::DockBuilderDockWindow("Sensor Module##Atta", dockIdLeft);

        // Default docking for editor windows
        ImGui::DockBuilderDockWindow("##AttaToolbar", dockIdUp);
        ImGui::DockBuilderDockWindow("Log##Atta", dockIdDown);
        ImGui::DockBuilderDockWindow("Scene##Atta", dockIdRight);
        ImGui::DockBuilderFinish(_viewportDockId);
    }
}

void Editor::renderCameraWindows() {
    // TODO think another way to show camera windows
    std::vector<sensor::CameraInfo>& cameras = sensor::getCameraInfos();
    for (uint32_t i = 0; i < cameras.size(); i++) {
        if (cameras[i].showWindow) {
            component::Name* name = component::getComponent<component::Name>(cameras[i].entity);
            std::string windowName = name != nullptr ? name->name : "Camera";
            ImGui::Begin((windowName + "##CameraWindow" + std::to_string(cameras[i].entity)).c_str(), &(cameras[i].showWindow));
            {
                ImVec2 size = ImVec2(cameras[i].renderer->getWidth(), cameras[i].renderer->getHeight());
                ImGui::Image((ImTextureID)(intptr_t)cameras[i].renderer->getImGuiTexture(), size);
            }
            ImGui::End();
        }
    }
}

} // namespace atta::ui
