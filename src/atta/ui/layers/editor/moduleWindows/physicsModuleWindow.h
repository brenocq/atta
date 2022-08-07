//--------------------------------------------------
// Atta UI Module
// physicsModuleWindow.h
// Date: 2021-12-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_LAYERS_EDITOR_WINDOWS_PHYSICS_MODULE_WINDOW_H
#define ATTA_UI_LAYERS_EDITOR_WINDOWS_PHYSICS_MODULE_WINDOW_H

#include <atta/physics/manager.h>
#include <atta/ui/layers/editor/window.h>

namespace atta::ui {

class PhysicsModuleWindow : public Window<PhysicsModuleWindow> {
  private:
    PhysicsModuleWindow();
    void renderImpl();

    friend Window<PhysicsModuleWindow>;
};

} // namespace atta::ui

#endif // ATTA_UI_LAYERS_EDITOR_WINDOWS_PHYSICS_MODULE_WINDOW_H
