//--------------------------------------------------
// Atta UI System
// dockSpace.h
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_SYSTEM_LAYERS_EDITOR_DOCK_SPACE_H
#define ATTA_UI_SYSTEM_LAYERS_EDITOR_DOCK_SPACE_H

namespace atta::ui
{
    class DockSpace
    {
    public:
        void render();

        unsigned getViewportDockId() { return _viewportDockId; }
    private:
        unsigned _viewportDockId;
    };
}

#endif// ATTA_UI_SYSTEM_LAYERS_EDITOR_DOCK_SPACE_H
