//--------------------------------------------------
// Atta UI Module
// physicsDrawer.cpp
// Date: 2021-12-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/boxCollider.h>
#include <atta/component/components/sphereCollider.h>
#include <atta/component/components/transform.h>
#include <atta/component/interface.h>
#include <atta/graphics/drawer.h>
#include <atta/physics/interface.h>
#include <atta/ui/layers/editor/drawers/physicsDrawer.h>

namespace atta::ui {

void PhysicsDrawer::update() {
    graphics::Drawer::clear<graphics::Drawer::Line>("atta::ui::PhysicsDrawer");

    std::vector<component::EntityId> entities = component::getEntitiesView();
    for (auto entity : entities) {
        // Get transform
        auto t = component::getComponent<component::Transform>(entity);
        if (!t)
            continue;
        vec3 position, scale;
        quat orientation;
        mat4 m = t->getWorldTransform(entity);
        m.getPosOriScale(position, orientation, scale);

        // Choose color
        vec4 color = {0, 1, 0, 1};

        // Draw box collider
        auto box = component::getComponent<component::BoxCollider>(entity);
        if (box) {
            // Get vertices
            std::vector<vec3> vertices;
            switch (physics::getSelectedEngine()) {
                case physics::Engine::BOX2D: {
                    vertices = {{0.5, 0.5, 0}, {-0.5, 0.5, 0}, {-0.5, -0.5, 0}, {0.5, -0.5, 0}};
                    break;
                }
                case physics::Engine::BULLET: {
                    vertices = {{0.5, 0.5, 0.5},  {-0.5, 0.5, 0.5},  {-0.5, -0.5, 0.5},  {0.5, -0.5, 0.5},
                                {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}};
                    break;
                }
            }

            mat3 rotmat = orientation.getRotationMatrix();
            for (auto& v : vertices) {
                // Scale
                v *= box->size * scale;
                // Rotate
                v = rotmat * v;
                // Translate
                v += box->offset + position;
            }

            if (vertices.size() == 4) {
                graphics::Drawer::add(graphics::Drawer::Line(vertices[0], vertices[1], color, color), "atta::ui::PhysicsDrawer");
                graphics::Drawer::add(graphics::Drawer::Line(vertices[1], vertices[2], color, color), "atta::ui::PhysicsDrawer");
                graphics::Drawer::add(graphics::Drawer::Line(vertices[2], vertices[3], color, color), "atta::ui::PhysicsDrawer");
                graphics::Drawer::add(graphics::Drawer::Line(vertices[3], vertices[0], color, color), "atta::ui::PhysicsDrawer");
            } else if (vertices.size() == 8) {
                // Top
                graphics::Drawer::add(graphics::Drawer::Line(vertices[0], vertices[1], color, color), "atta::ui::PhysicsDrawer");
                graphics::Drawer::add(graphics::Drawer::Line(vertices[1], vertices[2], color, color), "atta::ui::PhysicsDrawer");
                graphics::Drawer::add(graphics::Drawer::Line(vertices[2], vertices[3], color, color), "atta::ui::PhysicsDrawer");
                graphics::Drawer::add(graphics::Drawer::Line(vertices[3], vertices[0], color, color), "atta::ui::PhysicsDrawer");
                // Down
                graphics::Drawer::add(graphics::Drawer::Line(vertices[4], vertices[5], color, color), "atta::ui::PhysicsDrawer");
                graphics::Drawer::add(graphics::Drawer::Line(vertices[5], vertices[6], color, color), "atta::ui::PhysicsDrawer");
                graphics::Drawer::add(graphics::Drawer::Line(vertices[6], vertices[7], color, color), "atta::ui::PhysicsDrawer");
                graphics::Drawer::add(graphics::Drawer::Line(vertices[7], vertices[4], color, color), "atta::ui::PhysicsDrawer");
                // Connection
                graphics::Drawer::add(graphics::Drawer::Line(vertices[0], vertices[4], color, color), "atta::ui::PhysicsDrawer");
                graphics::Drawer::add(graphics::Drawer::Line(vertices[1], vertices[5], color, color), "atta::ui::PhysicsDrawer");
                graphics::Drawer::add(graphics::Drawer::Line(vertices[2], vertices[6], color, color), "atta::ui::PhysicsDrawer");
                graphics::Drawer::add(graphics::Drawer::Line(vertices[3], vertices[7], color, color), "atta::ui::PhysicsDrawer");
            }
        }

        // Draw circle collider
        auto circle = component::getComponent<component::SphereCollider>(entity);
        if (circle) {
            const unsigned numVertices = 32;
            std::vector<vec3> vertices;
            for (unsigned i = 0; i < numVertices; i++) {
                float angle = i / float(numVertices) * 2 * 3.1415926535f;
                vertices.push_back(vec3(cos(angle), sin(angle), 0.0f));
            }

            for (auto& v : vertices)
            {
                // Scale
                v *= circle->radius;
                v *= std::max(scale.x, scale.y);
                // Translate
                v += circle->offset;
                v += position;
            }

            for (unsigned i = 0; i < numVertices; i++)
                graphics::Drawer::add(graphics::Drawer::Line(vertices[i], vertices[(i - 1) % numVertices], color, color), "atta::ui::PhysicsDrawer");
        }
    }
}

} // namespace atta::ui
