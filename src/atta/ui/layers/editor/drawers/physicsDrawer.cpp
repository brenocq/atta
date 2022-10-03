//--------------------------------------------------
// Atta UI Module
// physicsDrawer.cpp
// Date: 2021-12-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/boxCollider.h>
#include <atta/component/components/rigidBody.h>
#include <atta/component/components/sphereCollider.h>
#include <atta/component/components/transform.h>
#include <atta/component/interface.h>
#include <atta/graphics/drawer.h>
#include <atta/physics/engines/bulletEngine.h>
#include <atta/physics/interface.h>
#include <atta/ui/layers/editor/drawers/physicsDrawer.h>
#include <atta/utils/config.h>

namespace atta::ui {

void PhysicsDrawer::update() {
    graphics::Drawer::clear<graphics::Drawer::Line>("atta::ui::PhysicsDrawer");
    // Show colliders
    if (physics::getShowColliders()) {
        std::vector<component::EntityId> entities = component::getEntitiesView();
        for (auto entity : entities) {
            // Get transform
            auto t = component::getComponent<component::Transform>(entity);
            if (!t)
                continue;

            mat4 worldTrans = t->getWorldTransform(entity);
            vec3 pos, scale;
            quat ori;
            worldTrans.getPosOriScale(pos, ori, scale);

            // Base color
            vec4 color = {1, 1, 1, 1};
            // Change color based on bullet rigid body state
            if (physics::getEngineType() == physics::Engine::BULLET && Config::getState() != Config::State::IDLE) {
                auto bullet = physics::getEngine<physics::BulletEngine>();
                auto rb = bullet->getBulletRigidBody(entity);
                if (rb)
                    color = rb->isActive() ? vec4(0, 1, 0, 1) : vec4(1, 1, 0, 1);
            }

            // Draw box collider
            auto box = component::getComponent<component::BoxCollider>(entity);
            if (box) {
                switch (physics::getEngineType()) {
                    case physics::Engine::BOX2D:
                        drawSquare(pos + box->offset, ori, scale * box->size, color);
                        break;
                    case physics::Engine::BULLET:
                        drawBox(pos + box->offset, ori, scale * box->size, color);
                        break;
                }
            }

            // Draw sphere collider
            auto sphere = component::getComponent<component::SphereCollider>(entity);
            if (sphere) {
                switch (physics::getEngineType()) {
                    case physics::Engine::BOX2D:
                        drawCircle(pos + sphere->offset, ori, scale * sphere->radius, color);
                        break;
                    case physics::Engine::BULLET:
                        drawSphere(pos + sphere->offset, ori, scale * sphere->radius, color);
                        break;
                }
            }
        }
    }

    // Show joints
    if (physics::getShowJoints()) {
        std::vector<component::EntityId> entities = component::getEntitiesView();
        for (auto entity : entities) {
            auto p = component::getComponent<component::PrismaticJoint>(entity);

            if (p) {
                auto tA = component::getComponent<component::Transform>(p->bodyA);
                auto tB = component::getComponent<component::Transform>(p->bodyB);
                auto rbA = component::getComponent<component::RigidBody>(p->bodyA);
                auto rbB = component::getComponent<component::RigidBody>(p->bodyB);

                if (tA && tB && rbA && rbB) {
                    mat4 wTransA = tA->getWorldTransform(p->bodyA);
                    mat4 wTransB = tB->getWorldTransform(p->bodyB);

                    vec3 wAnchorA = wTransA * vec4(p->anchorA, 1);
                    vec3 wAnchorB = wTransB * vec4(p->anchorB, 1);
                    vec3 wAxisA = wTransA * vec4(p->axisA, 0);
                    vec3 wAxisB = wTransB * vec4(p->axisB, 0);
                    wAxisA.normalize();
                    wAxisB.normalize();

                    vec4 color = {0, 0, 1, 1};
                    graphics::Drawer::add(graphics::Drawer::Line(wAnchorA, wAnchorB, color, color), "atta::ui::PhysicsDrawer");
                    quat oriA;
                    oriA.setRotationFromVectors(wAxisA, vec3(0, 0, 1));
                    quat oriB;
                    oriB.setRotationFromVectors(wAxisB, vec3(0, 0, 1));

                    drawSquare(wAnchorA, oriA, vec3(0.1), color);
                    drawSquare(wAnchorB, oriB, vec3(0.1), color);

                    if (p->enableLimits) {
                        drawSquare(wAnchorA + wAxisA*p->lowerTranslation, oriA, vec3(0.05), color);
                        drawSquare(wAnchorA + wAxisA*p->upperTranslation, oriA, vec3(0.05), color);
                    }
                }
            }
        }
    }

    // Bullet
    if (physics::getEngineType() == physics::Engine::BULLET && Config::getState() != Config::State::IDLE) {
        std::shared_ptr<physics::BulletEngine> bullet = std::static_pointer_cast<physics::BulletEngine>(physics::getEngine());
        // Draw AABBs
        if (bullet->getShowAabb()) {
            std::vector<component::EntityId> entities = component::getEntitiesView();
            for (auto entity : entities) {
                // Check if entity has bullet rigid body
                if (bullet->getBulletRigidBody(entity)) {
                    bnd3 aabb = bullet->getAabb(entity);
                    vec3 center = (aabb.pMin + aabb.pMax) / 2.0f;
                    vec3 size = aabb.pMax - aabb.pMin;
                    drawBox(center, quat(), size, {0.3, 0.8, 1.0, 1.0});
                }
            }
        }

        // Draw contacts
        if (physics::getShowContacts()) {
            auto contacts = bullet->getCollisions();
            for (auto [bodyA, bodiesB] : contacts) {
                for (auto [bodyB, manifold] : bodiesB) {
                    if (bodyA < bodyB) {
                        int numContacts = manifold->getNumContacts();
                        for (int i = 0; i < numContacts; i++) {
                            const btManifoldPoint& contactPoint = manifold->getContactPoint(i);
                            btVector3 btp0 = contactPoint.getPositionWorldOnA();
                            btVector3 btp1 = contactPoint.getPositionWorldOnB();
                            btVector3 n = contactPoint.m_normalWorldOnB;
                            vec3 p0(btp0.x(), btp0.y(), btp0.z());
                            vec3 p1(btp1.x(), btp1.y(), btp1.z());
                            vec4 color(1, 1, 1, 1);
                            vec3 normal(n.x(), n.y(), n.z());
                            quat ori;
                            ori.setRotationFromVectors(normal, vec3(0, 0, 1));
                            drawCircle((p0 + p1) / 2, ori, vec3(0.1), color);
                        }
                    }
                }
            }
        }
    }
}

void PhysicsDrawer::drawSquare(vec3 position, quat orientation, vec3 scale, vec4 color) {
    std::vector<vec3> vertices = {{0.5, 0.5, 0}, {-0.5, 0.5, 0}, {-0.5, -0.5, 0}, {0.5, -0.5, 0}};

    mat4 mat;
    mat.setPosOriScale(position, orientation, scale);
    for (auto& v : vertices)
        v = vec3(mat * vec4(v, 1));

    graphics::Drawer::add(graphics::Drawer::Line(vertices[0], vertices[1], color, color), "atta::ui::PhysicsDrawer");
    graphics::Drawer::add(graphics::Drawer::Line(vertices[1], vertices[2], color, color), "atta::ui::PhysicsDrawer");
    graphics::Drawer::add(graphics::Drawer::Line(vertices[2], vertices[3], color, color), "atta::ui::PhysicsDrawer");
    graphics::Drawer::add(graphics::Drawer::Line(vertices[3], vertices[0], color, color), "atta::ui::PhysicsDrawer");
}

void PhysicsDrawer::drawBox(vec3 position, quat orientation, vec3 scale, vec4 color) {
    std::vector<vec3> vertices = {{0.5, 0.5, 0.5},  {-0.5, 0.5, 0.5},  {-0.5, -0.5, 0.5},  {0.5, -0.5, 0.5},
                                  {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}};

    mat4 mat;
    mat.setPosOriScale(position, orientation, scale);
    for (auto& v : vertices)
        v = vec3(mat * vec4(v, 1));

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

void PhysicsDrawer::drawCircle(vec3 position, quat orientation, vec3 scale, vec4 color) {
    const unsigned numVertices = 32;
    std::vector<vec3> vertices;
    for (unsigned i = 0; i < numVertices; i++) {
        float angle = i / float(numVertices) * 2 * M_PI;
        vertices.push_back(vec3(cos(angle), sin(angle), 0.0f));
    }

    mat4 mat;
    mat.setPosOriScale(position, orientation, scale);
    for (auto& v : vertices)
        v = vec3(mat * vec4(v, 1));

    for (unsigned i = 0; i < numVertices; i++)
        graphics::Drawer::add(graphics::Drawer::Line(vertices[i], vertices[(i - 1) % numVertices], color, color), "atta::ui::PhysicsDrawer");
}

void PhysicsDrawer::drawSphere(vec3 position, quat orientation, vec3 scale, vec4 color) {
    const unsigned numVertices = 32;
    std::vector<vec3> vertices;
    for (unsigned i = 0; i < numVertices; i++) {
        float angle = i / float(numVertices) * 2 * M_PI;
        vertices.push_back(vec3(0, cos(angle), sin(angle)));
        vertices.push_back(vec3(cos(angle), 0, sin(angle)));
        vertices.push_back(vec3(cos(angle), sin(angle), 0));
    }

    mat4 mat;
    mat.setPosOriScale(position, orientation, scale);
    for (auto& v : vertices)
        v = vec3(mat * vec4(v, 1));

    for (unsigned i = 0; i < (numVertices - 1) * 3; i += 3) {
        graphics::Drawer::add(graphics::Drawer::Line(vertices[i], vertices[(i + 3) % (numVertices * 3)], color, color), "atta::ui::PhysicsDrawer");
        graphics::Drawer::add(graphics::Drawer::Line(vertices[i + 1], vertices[(i + 1 + 3) % (numVertices * 3)], color, color),
                              "atta::ui::PhysicsDrawer");
        graphics::Drawer::add(graphics::Drawer::Line(vertices[i + 2], vertices[(i + 2 + 3) % (numVertices * 3)], color, color),
                              "atta::ui::PhysicsDrawer");
    }
}

} // namespace atta::ui
