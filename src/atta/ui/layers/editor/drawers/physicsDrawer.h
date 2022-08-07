//--------------------------------------------------
// Atta UI Module
// physicsDrawer.h
// Date: 2021-12-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_LAYERS_EDITOR_DRAWERS_PHYSICS_DRAWER_H
#define ATTA_UI_LAYERS_EDITOR_DRAWERS_PHYSICS_DRAWER_H

namespace atta::ui {
/// Drawer used to draw physics collider lines
class PhysicsDrawer {
  public:
    void update(); ///< Update physics collider lines
};
} // namespace atta::ui

#endif // ATTA_UI_LAYERS_EDITOR_DRAWERS_PHYSICS_DRAWER_H
