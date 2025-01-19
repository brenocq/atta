//--------------------------------------------------
// Atta UI Module
// guizmo.h
// Date: 2024-01-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_WIDGETS_GUIZMO_H
#define ATTA_UI_WIDGETS_GUIZMO_H

#include <atta/component/interface.h>
#include <atta/ui/windows/viewport/viewport.h>

namespace atta::ui {

class Gizmo {
  public:
    enum Operation {
        TRANSLATE = 1 << 0,
        ROTATE = 1 << 1,
        SCALE = 1 << 2,
        TRANSLATE_ROTATE = TRANSLATE | ROTATE,
        TRANSLATE_SCALE = TRANSLATE | SCALE,
        ROTATE_SCALE = ROTATE | SCALE,
        ALL = TRANSLATE | ROTATE | SCALE
    };
    friend inline Operation operator|(Operation l, Operation r) { return Operation(int(l) | int(r)); }
    enum Mode { WORLD = 0, LOCAL };

    Gizmo();

    void setOperation(Operation operation);
    void setMode(Mode mode);
    void setViewport(std::weak_ptr<Viewport> viewport);
    void setSnap(bool snap);

    bool manipulate(component::EntityId entity);

  private:
    Operation _operation;
    Mode _mode;
    std::weak_ptr<Viewport> _viewport;
    bool _snap;
};

} // namespace atta::ui

#endif // ATTA_UI_WIDGETS_GUIZMO_H
