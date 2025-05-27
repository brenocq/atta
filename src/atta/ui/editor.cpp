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

#include <atta/ui/panels/menuBar/localWindows/versionWindow.h>
#include <atta/ui/windows/graphicsModuleWindow.h>
#include <atta/ui/windows/ioModuleWindow.h>
#include <atta/ui/windows/outputWindow.h>
#include <atta/ui/windows/physicsModuleWindow.h>
#include <atta/ui/windows/sceneWindow.h>
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
    // ImGui::ShowDemoWindow();
    // ImPlot::ShowDemoWindow();
    // ImPlot3D::ShowDemoWindow();

    // Panels
    _menuBar.render();
    _toolBar.render();
    _statusBar.render();

    // Drawers
    _physicsDrawer.update();
    _sensorDrawer.update();

    // Module windows
    GraphicsModuleWindow::render();
    IOModuleWindow::render();
    PhysicsModuleWindow::render();
    SensorModuleWindow::render();

    // Editor windows
    _viewportWindows.renderUI();
    SceneWindow::render();
    OutputWindow::render();
    TimeProfilerWindow::render();

    // Windows utils
    FileSelectionWindow::render();

    // MenuBar windows
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
    _rootDockId = ImGui::GetID("##AttaDockSpace");
    ImGui::DockSpaceOverViewport(_rootDockId, viewport, ImGuiDockNodeFlags_NoWindowMenuButton);

    //----- Create DockSpace nodes -----//
    static bool firstRender = true;
    if (firstRender) {
        firstRender = false;

        // Create main docking nodes
        ImGuiID dockIdStatus, dockIdToolbar, tempDockId;
        ImGui::DockBuilderSplitNode(_rootDockId, ImGuiDir_Down, 0.022f, &dockIdStatus, &tempDockId);
        ImGui::DockBuilderSplitNode(tempDockId, ImGuiDir_Left, 0.2f, &_leftDockId, &tempDockId);
        ImGui::DockBuilderSplitNode(tempDockId, ImGuiDir_Right, 0.2f, &_rightDockId, &tempDockId);
        ImGui::DockBuilderSplitNode(tempDockId, ImGuiDir_Down, 0.2f, &_downDockId, &tempDockId);
        ImGui::DockBuilderSplitNode(tempDockId, ImGuiDir_Up, 0.0f, &dockIdToolbar, &_centerDockId);

        // Disable split for toolbar/status bar nodes
        ImGuiDockNodeFlags noDockingFlags = ImGuiDockNodeFlags_NoDockingSplit | ImGuiDockNodeFlags_NoDockingOverCentralNode;
        ImGuiDockNode* rootNode = ImGui::DockBuilderGetNode(_rootDockId);
        ImGuiDockNode* tbNode = ImGui::DockBuilderGetNode(dockIdToolbar);
        ImGuiDockNode* sbNode = ImGui::DockBuilderGetNode(dockIdStatus);
        if (rootNode)
            rootNode->LocalFlags |= noDockingFlags;
        if (tbNode)
            tbNode->LocalFlags |= noDockingFlags;
        if (sbNode)
            sbNode->LocalFlags |= noDockingFlags;

        // Default docking for editor windows
        ImGui::DockBuilderDockWindow("##AttaPrivateToolBar", dockIdToolbar);
        ImGui::DockBuilderDockWindow("##AttaPrivateStatusBar", dockIdStatus);
        ImGui::DockBuilderFinish(_rootDockId);

        LOG_DEBUG("Editor", "Docking left: $0", _leftDockId);
        LOG_DEBUG("Editor", "Docking right: $0", _rightDockId);
        LOG_DEBUG("Editor", "Docking down: $0", _downDockId);
        LOG_DEBUG("Editor", "Docking center: $0", _centerDockId);
        LOG_DEBUG("Editor", "Docking root: $0", _rootDockId);
    }
}

void Editor::renderCameraWindows() {
    // TODO think another way to show camera windows
    std::vector<sensor::CameraInfo>& cameras = sensor::getCameraInfos();
    for (uint32_t i = 0; i < cameras.size(); i++) {
        if (cameras[i].showWindow) {
            component::Name* name = component::getComponent<component::Name>(cameras[i].entity);
            std::string windowName = name != nullptr ? name->name : "Camera";
            ImGui::Begin((windowName + "##AttaPrivateCameraWindow" + std::to_string(cameras[i].entity)).c_str(), &(cameras[i].showWindow));
            {
                // TODO it will crash if the renderer did not render yet
                // ImVec2 size = ImVec2(cameras[i].renderer->getWidth(), cameras[i].renderer->getHeight());
                // ImGui::Image((ImTextureID)(intptr_t)cameras[i].renderer->getImGuiTexture(), size);
                ImGui::Text("TODO :)");
            }
            ImGui::End();
        }
    }
}

} // namespace atta::ui
