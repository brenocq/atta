//--------------------------------------------------
// Atta UI Module
// dockSpace.h
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_LAYERS_EDITOR_DOCK_SPACE_H
#define ATTA_UI_LAYERS_EDITOR_DOCK_SPACE_H

namespace atta::ui {

class DockSpace {
  public:
    void render();

    unsigned getViewportDockId() { return _viewportDockId; }

  private:
    unsigned _viewportDockId;
};

} // namespace atta::ui

#endif // ATTA_UI_LAYERS_EDITOR_DOCK_SPACE_H
