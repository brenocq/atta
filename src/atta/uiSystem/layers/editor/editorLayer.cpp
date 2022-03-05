//--------------------------------------------------
// Atta UI System
// editorLayer.cpp
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/uiSystem/layers/editor/editorLayer.h>
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/resourceSystem/resourceManager.h>
#include <atta/scriptSystem/scriptManager.h>
#include <imgui_internal.h>

#include <atta/uiSystem/layers/editor/systemWindows/ioSystemWindow.h>
#include <atta/uiSystem/layers/editor/systemWindows/physicsSystemWindow.h>
#include <atta/uiSystem/layers/editor/windows/utils/fileSelectionWindow.h>

#include <atta/sensorSystem/sensorManager.h>

namespace atta::ui
{
    EditorLayer::EditorLayer():
        Layer(StringId("GraphicsEditorLayer"))
    {
    }

    void EditorLayer::onAttach()
    {
    }

    void EditorLayer::onDetach()
    {
    }

    void EditorLayer::onRender()
    {
    }

    void EditorLayer::onUIRender()
    {
        _dockSpace.render();

        // Demo
        //bool demo = true;
        //ImGui::ShowDemoWindow(&demo);

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

        // System windows
        IOSystemWindow::render();
        PhysicsSystemWindow::render();

        // Windows utils
        FileSelectionWindow::render();

        renderCameraWindows();

        // Project UI
        ProjectScript* project = ScriptManager::getProjectScript();
        if(project)
            project->onUIRender();
    }

    void EditorLayer::renderCameraWindows()
    {
        // TODO think another way to show camera windows
        std::vector<SensorManager::CameraInfo>& cameras = SensorManager::getCameraInfos();
        for(uint32_t i = 0; i < cameras.size(); i++)
        {
            if(cameras[i].showWindow)
            {
                ImGui::Begin(("Camera##CameraWindow"+std::to_string(cameras[i].entity)).c_str(), &(cameras[i].showWindow));
                {
                    ImVec2 size = ImVec2(cameras[i].renderer->getWidth(), cameras[i].renderer->getHeight());
                    ImGui::Image(cameras[i].renderer->getImGuiTexture(), size, ImVec2(0, 0), ImVec2(1, 1));
                }
                ImGui::End();
            }
        }
    }
}
