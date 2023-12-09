//--------------------------------------------------
// Atta UI Module
// editor.h
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_EDITOR_EDITOR_H
#define ATTA_UI_EDITOR_EDITOR_H

#include <atta/memory/allocatedObject.h>
#include <atta/ui/editor/dockSpace.h>
#include <atta/ui/editor/drawers/physicsDrawer.h>
#include <atta/ui/editor/drawers/sensorDrawer.h>
#include <atta/ui/editor/toolBar/toolBar.h>
#include <atta/ui/editor/topBar/topBar.h>
#include <atta/ui/editor/windows/entityWindow.h>
#include <atta/ui/editor/windows/logWindow.h>
#include <atta/ui/editor/windows/viewportWindows.h>

namespace atta::ui {

class Editor final {
  public:
    Editor();

    void render();

  private:
    void renderCameraWindows();

    DockSpace _dockSpace;
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

#endif // ATTA_UI_EDITOR_EDITOR_H
