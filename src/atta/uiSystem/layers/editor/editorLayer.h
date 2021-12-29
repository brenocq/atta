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
#include <atta/uiSystem/layers/editor/toolBar/toolBar.h>
#include <atta/uiSystem/layers/editor/windows/entityWindow.h>
#include <atta/uiSystem/layers/editor/windows/viewportWindows.h>
#include <atta/uiSystem/layers/editor/windows/logWindow.h>
#include <atta/uiSystem/layers/editor/drawers/physicsDrawer.h>

namespace atta::ui
{
    class EditorLayer final : public Layer, public AllocatedObject<EditorLayer, SID("GraphicsAllocator")>
    {
    public:
        EditorLayer();

        void onAttach() override;
        void onDetach() override;
        void onRender() override;
        void onUIRender() override;

    private:
        DockSpace _dockSpace;
        TopBar _topBar;
        ToolBar _toolBar;
        EntityWindow _entityWindow;
        LogWindow _logWindow;
        ViewportWindows _viewportWindows;

		// Drawers
		PhysicsDrawer _physicsDrawer;///< Physics drawer
    };
}

#endif// ATTA_UI_SYSTEM_LAYERS_EDITOR_EDITOR_LAYER_H
