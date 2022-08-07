//--------------------------------------------------
// Atta UI Module
// editorLayer.h
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_LAYERS_EDITOR_EDITOR_LAYER_H
#define ATTA_UI_LAYERS_EDITOR_EDITOR_LAYER_H
#include <atta/graphics/layers/layer.h>
#include <atta/memory/allocatedObject.h>
#include <atta/ui/layers/editor/dockSpace.h>
#include <atta/ui/layers/editor/drawers/physicsDrawer.h>
#include <atta/ui/layers/editor/drawers/sensorDrawer.h>
#include <atta/ui/layers/editor/toolBar/toolBar.h>
#include <atta/ui/layers/editor/topBar/topBar.h>
#include <atta/ui/layers/editor/windows/entityWindow.h>
#include <atta/ui/layers/editor/windows/logWindow.h>
#include <atta/ui/layers/editor/windows/viewportWindows.h>

namespace atta::ui {
class EditorLayer final : public Layer, public AllocatedObject<EditorLayer, SID("GraphicsAllocator")> {
  public:
    EditorLayer();

    void onAttach() override;
    void onDetach() override;
    void onRender() override;
    void onUIRender() override;

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

#endif // ATTA_UI_LAYERS_EDITOR_EDITOR_LAYER_H
