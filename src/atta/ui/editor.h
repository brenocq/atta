//--------------------------------------------------
// Atta UI Module
// editor.h
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_EDITOR_H
#define ATTA_UI_EDITOR_H

#include <atta/memory/allocatedObject.h>
#include <atta/ui/drawers/physicsDrawer.h>
#include <atta/ui/drawers/sensorDrawer.h>
#include <atta/ui/panels/toolBar/toolBar.h>
#include <atta/ui/panels/topBar/topBar.h>
#include <atta/ui/windows/entityWindow.h>
#include <atta/ui/windows/logWindow.h>
#include <atta/ui/windows/viewportWindows.h>

namespace atta::ui {

class Editor final {
  public:
    void render();

    unsigned getViewportDockId() { return _viewportDockId; }

  private:
    void setupDocking();
    void renderCameraWindows();

    // Docking
    unsigned _viewportDockId;
    bool _firstRender = true;

    TopBar _topBar;
    ToolBar _toolBar;
    EntityWindow _entityWindow;
    LogWindow _logWindow;
    ViewportWindows _viewportWindows;

    // Drawers
    PhysicsDrawer _physicsDrawer; ///< Physics drawer
    SensorDrawer _sensorDrawer;   ///< Sensor drawer
};

} // namespace atta::ui

#endif // ATTA_UI_EDITOR_H
