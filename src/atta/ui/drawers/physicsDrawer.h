//--------------------------------------------------
// Atta UI Module
// physicsDrawer.h
// Date: 2021-12-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_DRAWERS_PHYSICS_DRAWER_H
#define ATTA_UI_DRAWERS_PHYSICS_DRAWER_H

namespace atta::ui {

/// Drawer used to draw physics collider lines
class PhysicsDrawer {
  public:
    void update(); ///< Update physics collider lines
  private:
    void drawBullet();
    void drawBox2D();

    void drawSquare(vec3 position = {0, 0, 0}, quat orientation = {}, vec3 scale = {1, 1, 1}, vec4 color = {0, 1, 0, 1});
    void drawBox(vec3 position = {0, 0, 0}, quat orientation = {}, vec3 scale = {1, 1, 1}, vec4 color = {0, 1, 0, 1});
    void drawCircle(vec3 position = {0, 0, 0}, quat orientation = {}, vec3 scale = {1, 1, 1}, vec4 color = {0, 1, 0, 1});
    void drawSphere(vec3 position = {0, 0, 0}, quat orientation = {}, vec3 scale = {1, 1, 1}, vec4 color = {0, 1, 0, 1});
    void drawCylinder(vec3 position = {0, 0, 0}, quat orientation = {}, vec3 scale = {1, 1, 1}, vec4 color = {0, 1, 0, 1});
    void drawPolygon(std::vector<vec3> points = {0, 0, 0}, vec3 position = {0, 0, 0}, quat orientation = {}, vec3 scale = {1, 1, 1},
                     vec4 color = {0, 1, 0, 1});
};

} // namespace atta::ui

#endif // ATTA_UI_DRAWERS_PHYSICS_DRAWER_H
