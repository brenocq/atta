//--------------------------------------------------
// Atta UI Module
// editorLayer.cpp
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/interface.h>
#include <atta/resource/interface.h>
#include <atta/script/interface.h>
#include <atta/ui/layers/editor/editorLayer.h>
#include <imgui_internal.h>
#include <implot.h>

#include <atta/ui/layers/editor/moduleWindows/graphicsModuleWindow.h>
#include <atta/ui/layers/editor/moduleWindows/ioModuleWindow.h>
#include <atta/ui/layers/editor/moduleWindows/memoryModuleWindow.h>
#include <atta/ui/layers/editor/moduleWindows/parallelModuleWindow.h>
#include <atta/ui/layers/editor/moduleWindows/physicsModuleWindow.h>
#include <atta/ui/layers/editor/moduleWindows/sensorModuleWindow.h>
#include <atta/ui/layers/editor/tools/timeProfiler/timeProfilerWindow.h>
#include <atta/ui/layers/editor/topBar/localWindows/versionWindow.h>
#include <atta/ui/layers/editor/windows/utils/fileSelectionWindow.h>

#include <atta/component/components/name.h>
#include <atta/component/interface.h>
#include <atta/sensor/manager.h>

namespace atta::ui {

EditorLayer::EditorLayer() : graphics::Layer(StringId("GraphicsEditorLayer")) {}

void EditorLayer::onAttach() {}

void EditorLayer::onDetach() {}

void EditorLayer::onRender() {}

void EditorLayer::onUIRender() {
    PROFILE();
    _dockSpace.render();

    // Demo
    // bool demo = true;
    // ImGui::ShowDemoWindow(&demo);
    // ImPlot::ShowDemoWindow(&demo);

    // Top interface
    _topBar.render();
    _toolBar.render();

    // Drawers
    _physicsDrawer.update();
    _sensorDrawer.update();

    // Windows
    _entityWindow.render();
    _logWindow.render();
    _viewportWindows.render();

    // Module windows
    GraphicsModuleWindow::render();
    IOModuleWindow::render();
    MemoryModuleWindow::render();
    ParallelModuleWindow::render();
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
    try {
        script::WorldRegistry::onUIRender();
    } catch (std::exception& e) {
        LOG_ERROR("ui::EditorLayer", "Error when executing [w]script::World::onUIRender[]: $0", e.what());
    }
}

void EditorLayer::renderCameraWindows() {
    // TODO think another way to show camera windows
    std::vector<sensor::CameraInfo>& cameras = sensor::getCameraInfos();
    for (uint32_t i = 0; i < cameras.size(); i++) {
        if (cameras[i].showWindow) {
            component::Name* name = cameras[i].entity.get<component::Name>();
            std::string windowName = name != nullptr ? name->name : "Camera";
            ImGui::Begin((windowName + "##CameraWindow" + std::to_string(cameras[i].entity)).c_str(), &(cameras[i].showWindow));
            {
                ImVec2 size = ImVec2(cameras[i].renderer->getWidth(), cameras[i].renderer->getHeight());
                ImGui::Image(cameras[i].renderer->getImGuiTexture(), size, ImVec2(0, 0), ImVec2(1, 1));
            }
            ImGui::End();
        }
    }
}

} // namespace atta::ui
