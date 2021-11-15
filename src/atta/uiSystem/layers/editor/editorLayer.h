//--------------------------------------------------
// Atta UI System
// editorLayer.h
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_SYSTEM_LAYERS_EDITOR_EDITOR_LAYER_H
#define ATTA_UI_SYSTEM_LAYERS_EDITOR_EDITOR_LAYER_H
#include <atta/graphicsSystem/layers/layer.h>
#include <atta/memorySystem/allocatedObject.h>
#include <atta/uiSystem/layers/editor/dockSpace.h>
#include <atta/uiSystem/layers/editor/topBar/topBar.h>
#include <atta/uiSystem/layers/editor/windows/entityWindow.h>

namespace atta::ui
{
    class EditorLayer final : public Layer, public AllocatedObject<EditorLayer, SID("GraphicsAllocator")>
    {
    public:
        enum class EditorState
        {
            EDITOR = 0,
            SIMULATION_RUNNING,
            SIMULATION_PAUSED,
        };

        EditorLayer();

        void onAttach() override;
        void onDetach() override;
        void onRender() override;
        void onUIRender() override;

    private:
        void updateViewports();
        void toolbar();
        void addBasicShapePopup();
        void onSimulationStateChange(Event& event);

        DockSpace _dockSpace;
        TopBar _topBar;
        EntityWindow _entityWindow;

        EditorState _editorState;
    };
}

#endif// ATTA_UI_SYSTEM_LAYERS_EDITOR_EDITOR_LAYER_H
