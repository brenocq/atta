//--------------------------------------------------
// Atta UI Module
// guizmo.h
// Date: 2024-01-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_WIDGETS_GUIZMO_H
#define ATTA_UI_WIDGETS_GUIZMO_H

#include <atta/component/interface.h>
#include <atta/graphics/cameras/camera.h>

namespace atta::ui {

class Gizmo {
  public:
    enum Operation {
        TRANSLATE = 1 << 0,
        ROTATE = 1 << 1,
        SCALE = 1 << 2,
    };
    inline Operation operator|(OPERATION l, OPERATION r) { return Operation(int(l) | int(r)); }
    enum Mode { WORLD = 0, LOCAL };

    Gizmo();

    void setOperation(Operation operation);
    void setMode(Mode mode);
    void setCamera(std::weak_ptr<Camera> camera);
    void setSnap(bool snap);

    bool manipulate(component::EntityId entity);

  private:
    Operation _operation;
    Mode _mode;
    std::weak_ptr<Camera> _camera;
    bool _snap;
};

} // namespace atta::ui

#endif // ATTA_UI_WIDGETS_GUIZMO_H
