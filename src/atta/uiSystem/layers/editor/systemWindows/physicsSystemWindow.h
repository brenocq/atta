//--------------------------------------------------
// Atta UI System
// physicsSystemWindow.h
// Date: 2021-12-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_SYSTEM_LAYERS_EDITOR_WINDOWS_PHYSICS_SYSTEM_WINDOW_H
#define ATTA_UI_SYSTEM_LAYERS_EDITOR_WINDOWS_PHYSICS_SYSTEM_WINDOW_H
#include <atta/uiSystem/layers/editor/window.h>
#include <atta/physicsSystem/physicsManager.h>


namespace atta::ui
{
    class PhysicsSystemWindow : public Window<PhysicsSystemWindow>
    {
    private:
        PhysicsSystemWindow();
        void renderImpl();

        friend Window<PhysicsSystemWindow>;
    };
}

#endif// ATTA_UI_SYSTEM_LAYERS_EDITOR_WINDOWS_PHYSICS_SYSTEM_WINDOW_H
