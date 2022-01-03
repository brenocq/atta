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

        // Windows
        _entityWindow.render();
        _logWindow.render();
        _viewportWindows.render();

        // System windows
        IOSystemWindow::render();
        PhysicsSystemWindow::render();

        // Project UI
        ProjectScript* project = ScriptManager::getProjectScript();
        if(project)
            project->onUIRender();
    }
}
