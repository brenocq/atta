//--------------------------------------------------
// Atta UI System
// toolBar.h
// Date: 2021-12-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_SYSTEM_LAYERS_EDITOR_TOOL_BAR_TOOL_BAR_H
#define ATTA_UI_SYSTEM_LAYERS_EDITOR_TOOL_BAR_TOOL_BAR_H
#include <atta/eventSystem/event.h>

namespace atta::ui
{
    class ToolBar
    {
    public:
        ToolBar();

        void render();
        
    private:
        void handleShortcuts();
        void onSimulationStateChange(Event& event);

        enum class EditorState
        {
            EDITOR = 0,
            SIMULATION_RUNNING,
            SIMULATION_PAUSED,
        };
        void changeState(EditorState newState);

        EditorState _editorState;
    };
}

#endif// ATTA_UI_SYSTEM_LAYERS_EDITOR_TOOL_BAR_TOOL_BAR_H
