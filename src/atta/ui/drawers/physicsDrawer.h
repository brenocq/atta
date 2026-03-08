// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

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
