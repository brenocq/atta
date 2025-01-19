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
    bool demo = true;
    ImGui::ShowDemoWindow(&demo);
    // ImPlot::ShowDemoWindow(&demo);
    // ImPlot3D::ShowDemoWindow(&demo);

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

bool Editor::getViewportRendering() const { return _viewportWindows.getViewportRendering(); }
void Editor::setViewportRendering(bool viewportRendering) { _viewportWindows.setViewportRendering(viewportRendering); }

void Editor::setupDocking() {
    //----- Create DockSpace -----//
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    bool open = true;
    ImGui::Begin("EditorDockSpace", &open, window_flags);
    ImGui::PopStyleVar(3);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("EditorDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton);

        //----- Clear previous layout -----//
        if (_firstRender) {
            _firstRender = false;

            ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
            ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

            auto dock_id_scene = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.20f, nullptr, &dockspace_id);
            auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.20f, nullptr, &dockspace_id);
            auto dock_id_vp_top = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.00f, nullptr, &dockspace_id);

            // we now dock our windows into the docking node we made above
            _viewportDockId = dockspace_id;
            ImGui::DockBuilderDockWindow("Main Viewport###ViewportMain Viewport", dockspace_id);
            ImGui::DockBuilderDockWindow("Log", dock_id_down);
            ImGui::DockBuilderDockWindow("Scene", dock_id_scene);
            ImGui::DockBuilderDockWindow("##Toolbar", dock_id_vp_top);
            ImGui::DockBuilderFinish(dockspace_id);
        }
    }
    ImGui::End();
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
