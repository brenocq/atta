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
#include <atta/ui/panels/menuBar/menuBar.h>
#include <atta/ui/panels/statusBar/statusBar.h>
#include <atta/ui/panels/toolBar/toolBar.h>
#include <atta/ui/windows/viewport/viewportWindows.h>
#include <imgui.h>

namespace atta::ui {

class Editor final {
  public:
    void startUp();
    void shutDown();

    void renderViewports();
    void render();

    const std::vector<std::shared_ptr<ui::Viewport>>& getViewports() const;
    void openViewportModal(StringId sid);
    void clearViewports();
    void addViewport(std::shared_ptr<ui::Viewport> viewport);
    bool getViewportRendering() const;
    void setViewportRendering(bool viewportRendering);

    ImGuiID getRootDockId() const { return _rootDockId; }
    ImGuiID getCenterDockId() const { return _centerDockId; }
    ImGuiID getLeftDockId() const { return _leftDockId; }
    ImGuiID getRightDockId() const { return _rightDockId; }
    ImGuiID getDownDockId() const { return _downDockId; }

  private:
    void setupDocking();
    void renderCameraWindows();

    // Docking
    ImGuiID _rootDockId;
    ImGuiID _centerDockId;
    ImGuiID _leftDockId;
    ImGuiID _rightDockId;
    ImGuiID _downDockId;

    MenuBar _menuBar;
    ToolBar _toolBar;
    StatusBar _statusBar;
    ViewportWindows _viewportWindows;

    // Drawers
    PhysicsDrawer _physicsDrawer; ///< Physics drawer
    SensorDrawer _sensorDrawer;   ///< Sensor drawer
};

} // namespace atta::ui

#endif // ATTA_UI_EDITOR_H
