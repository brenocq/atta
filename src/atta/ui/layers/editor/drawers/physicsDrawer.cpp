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
            std::vector<vec3> vertices = {{0.5, 0.5, 0}, {-0.5, 0.5, 0}, {-0.5, -0.5, 0}, {0.5, -0.5, 0}};

            // Scale
            for (auto& v : vertices)
                v *= box->size;
            for (auto& v : vertices)
                v *= scale;

            // Rotate
            vec3 euler = orientation.toEuler();
            for (auto& v : vertices) {
                vec3 oldv = v;
                v.x = oldv.x * cos(-euler.z) - oldv.y * sin(-euler.z);
                v.y = oldv.x * sin(-euler.z) + oldv.y * cos(-euler.z);
            }

            // Translate
            for (auto& v : vertices)
                v += box->offset;
            for (auto& v : vertices)
                v += position;

            graphics::Drawer::add(graphics::Drawer::Line(vertices[0], vertices[1], color, color), "atta::ui::PhysicsDrawer");
            graphics::Drawer::add(graphics::Drawer::Line(vertices[1], vertices[2], color, color), "atta::ui::PhysicsDrawer");
            graphics::Drawer::add(graphics::Drawer::Line(vertices[2], vertices[3], color, color), "atta::ui::PhysicsDrawer");
            graphics::Drawer::add(graphics::Drawer::Line(vertices[3], vertices[0], color, color), "atta::ui::PhysicsDrawer");
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

            // Scale
            for (auto& v : vertices)
                v *= circle->radius;
            for (auto& v : vertices)
                v *= std::max(scale.x, scale.y);

            // Translate
            for (auto& v : vertices)
                v += circle->offset;
            for (auto& v : vertices)
                v += position;

            for (unsigned i = 0; i < numVertices; i++)
                graphics::Drawer::add(graphics::Drawer::Line(vertices[i], vertices[(i - 1) % numVertices], color, color), "atta::ui::PhysicsDrawer");
        }
    }
}

} // namespace atta::ui
