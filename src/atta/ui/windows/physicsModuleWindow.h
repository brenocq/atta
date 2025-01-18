//--------------------------------------------------
// Atta UI Module
// physicsModuleWindow.h
// Date: 2021-12-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_WINDOWS_PHYSICS_MODULE_WINDOW_H
#define ATTA_UI_WINDOWS_PHYSICS_MODULE_WINDOW_H

#include <atta/physics/interface.h>
#include <atta/ui/editor/window.h>

namespace atta::ui {

class PhysicsModuleWindow : public Window<PhysicsModuleWindow> {
  private:
    PhysicsModuleWindow();
    void renderImpl();

    friend Window<PhysicsModuleWindow>;
};

} // namespace atta::ui

#endif // ATTA_UI_WINDOWS_PHYSICS_MODULE_WINDOW_H
