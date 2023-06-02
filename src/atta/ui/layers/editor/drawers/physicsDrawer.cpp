//--------------------------------------------------
// Atta UI Module
// physicsDrawer.cpp
// Date: 2021-12-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/layers/editor/drawers/physicsDrawer.h>

#include <atta/component/components/boxCollider.h>
#include <atta/component/components/boxCollider2D.h>
#include <atta/component/components/circleCollider2D.h>
#include <atta/component/components/cylinderCollider.h>
#include <atta/component/components/prototype.h>
#include <atta/component/components/rigidBody.h>
#include <atta/component/components/sphereCollider.h>
#include <atta/component/components/transform.h>
#include <atta/component/interface.h>
#include <atta/graphics/drawer.h>
#include <atta/physics/engines/bulletEngine.h>
#include <atta/physics/interface.h>
#include <atta/processor/interface.h>

namespace atta::ui {

void PhysicsDrawer::update() {
    graphics::Drawer::clear<graphics::Drawer::Line>("atta::ui::PhysicsDrawer");

    switch (physics::getEngineType()) {
        case physics::Engine::BULLET:
            drawBullet();
            break;
        case physics::Engine::BOX2D:
            drawBox2D();
            break;
    }
}

void PhysicsDrawer::drawBullet() {
    //---------- Show colliders ----------//
    if (physics::getShowColliders()) {
        std::vector<component::Entity> entities = component::getEntitiesView();
        for (auto entity : entities) {
            // Get transform
            auto t = entity.get<component::Transform>();
            if (!t)
                continue;

            if (component::Entity(entity).isPrototype())
                continue;

            component::Transform worldTrans = t->getWorldTransform(entity);
            vec3 pos = worldTrans.position;
            vec3 scale = worldTrans.scale;
            quat ori = worldTrans.orientation;

            // Base color
            vec4 color = {1, 1, 1, 1};

            // Change color based on bullet rigid body state
            if (processor::getState() != processor::State::IDLE) {
                auto bullet = physics::getEngine<physics::BulletEngine>();
                auto rb = bullet->getBulletRigidBody(entity);
                if (rb)
                    color = rb->isActive() ? vec4(0, 1, 0, 1) : vec4(1, 1, 0, 1);
            }

            //----- Box collider -----//
            auto box = entity.get<component::BoxCollider>();
            if (box)
                drawBox(pos + box->offset, ori, scale * box->size, color);

            //----- Sphere collider -----//
            auto sphere = entity.get<component::SphereCollider>();
            if (sphere)
                drawSphere(pos + sphere->offset, ori, scale * sphere->radius, color);

            //----- Cylinder collider -----//
            auto cylinder = entity.get<component::CylinderCollider>();
            if (cylinder && physics::getEngineType() == physics::Engine::BULLET)
                drawCylinder(pos + cylinder->offset, ori, vec3(vec2(scale) * cylinder->radius * 2, scale.z * cylinder->height), color);
        }
    }

    //---------- Show joints ----------//
    if (physics::getShowJoints()) {
        std::vector<component::Entity> entities = component::getEntitiesView();
        for (auto entity : entities) {
            auto p = entity.get<component::PrismaticJoint>();

            if (component::Entity(entity).isPrototype())
                continue;

            //----- Prismatic joint -----//
            if (p) {
                auto tA = p->bodyA.get<component::Transform>();
                auto tB = p->bodyB.get<component::Transform>();
                auto rbA = p->bodyA.get<component::RigidBody>();
                auto rbB = p->bodyB.get<component::RigidBody>();

                if (tA && tB && rbA && rbB) {
                    mat4 wTransA = tA->getWorldTransformMatrix(p->bodyA);
                    mat4 wTransB = tB->getWorldTransformMatrix(p->bodyB);

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
                        drawSquare(wAnchorA + wAxisA * p->lowerLimit, oriA, vec3(0.05), color);
                        drawSquare(wAnchorA + wAxisA * p->upperLimit, oriA, vec3(0.05), color);
                    }
                }
            }

            //----- Revolute joint -----//
            auto r = entity.get<component::RevoluteJoint>();
            if (r) {
                auto tA = r->bodyA.get<component::Transform>();
                auto tB = r->bodyB.get<component::Transform>();
                auto rbA = r->bodyA.get<component::RigidBody>();
                auto rbB = r->bodyB.get<component::RigidBody>();

                if (tA && tB && rbA && rbB) {
                    mat4 wTransA = tA->getWorldTransformMatrix(r->bodyA);
                    mat4 wTransB = tB->getWorldTransformMatrix(r->bodyB);

                    // TODO anchor position does not match bullet engine position
                    vec3 wAnchorA = wTransA * vec4(r->anchorA, 1);
                    vec3 wAnchorB = wTransB * vec4(r->anchorB, 1);
                    vec3 wAxisA = wTransA * vec4(r->axisA, 0);
                    vec3 wAxisB = wTransB * vec4(r->axisB, 0);
                    wAxisA.normalize();
                    wAxisB.normalize();

                    vec4 color = {0, 0, 1, 1};
                    graphics::Drawer::add(graphics::Drawer::Line(wAnchorA, wAnchorB, color, color), "atta::ui::PhysicsDrawer");
                    quat oriA;
                    oriA.setRotationFromVectors(wAxisA, vec3(0, 0, 1));
                    quat oriB;
                    oriB.setRotationFromVectors(wAxisB, vec3(0, 0, 1));

                    drawCircle(wAnchorA, oriA, vec3(0.1), color);
                    drawCircle(wAnchorB, oriB, vec3(0.1), color);
                }
            }
        }
    }

    if (processor::getState() != processor::State::IDLE) {
        std::shared_ptr<physics::BulletEngine> bullet = std::static_pointer_cast<physics::BulletEngine>(physics::getEngine());
        //---------- Draw AABBs ----------//
        if (bullet->getShowAabb()) {
            std::vector<component::Entity> entities = component::getEntitiesView();
            for (auto entity : entities) {
                if (component::Entity(entity).isPrototype())
                    continue;

                // Check if entity has bullet rigid body
                if (bullet->getBulletRigidBody(entity)) {
                    bnd3 aabb = bullet->getAabb(entity);
                    vec3 center = (aabb.pMin + aabb.pMax) / 2.0f;
                    vec3 size = aabb.pMax - aabb.pMin;
                    drawBox(center, quat(), size, {0.3, 0.8, 1.0, 1.0});
                }
            }
        }

        //---------- Draw contacts ----------//
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

void PhysicsDrawer::drawBox2D() {
    //---------- Show colliders ----------//
    if (physics::getShowColliders()) {
        // Base color
        vec4 color = {1, 1, 1, 1};

        std::vector<component::Entity> entities = component::getEntitiesView();
        for (auto entity : entities) {
            if (component::Entity(entity).isPrototype())
                continue;

            // Change color based on box2d rigid body state
            if (processor::getState() != processor::State::IDLE) {
                auto box2d = physics::getEngine<physics::Box2DEngine>();
                auto rb = box2d->getBox2DRigidBody(entity);
                if (rb)
                    color = rb->IsAwake() ? vec4(0, 1, 0, 1) : vec4(1, 1, 0, 1);
            }

            // Get transform
            auto t = entity.get<component::Transform>();
            if (!t)
                continue;
            component::Transform worldTrans = t->getWorldTransform(entity);
            vec3 pos = worldTrans.position;
            vec3 scale = worldTrans.scale;
            quat ori = worldTrans.orientation;

            //----- Box collider 2D -----//
            auto box = entity.get<component::BoxCollider2D>();
            if (box)
                drawSquare(pos + vec3(box->offset, 0.0f), ori, scale * vec3(box->size, 1), color);

            //----- Circle collider 2D -----//
            auto circle = entity.get<component::CircleCollider2D>();
            if (circle)
                drawCircle(pos + vec3(circle->offset, 0.0f), ori, scale * vec3(vec2(circle->radius), 1), color);
        }
    }

    //---------- Show joints ----------//
    if (physics::getShowJoints()) {
    }

    //---------- Draw contacts ----------//
    if (physics::getShowContacts()) {
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

void PhysicsDrawer::drawCylinder(vec3 position, quat orientation, vec3 scale, vec4 color) {
    vec3 axisX = {1, 0, 0};
    vec3 axisY = {0, 1, 0};
    vec3 axisZ = {0, 0, 1};
    orientation.rotateVector(axisX);
    orientation.rotateVector(axisY);
    orientation.rotateVector(axisZ);
    vec3 top = position + axisZ * scale.z * 0.5;
    vec3 bottom = position - axisZ * scale.z * 0.5;
    drawCircle(top, orientation, scale * 0.5, color);
    drawCircle(bottom, orientation, scale * 0.5, color);
    for (vec3 axis : std::vector<vec3>{axisX, axisY})
        for (int i : std::vector<int>{-1, 1})
            graphics::Drawer::add(graphics::Drawer::Line(bottom + axis * scale.x * 0.5 * i, top + axis * scale.x * 0.5 * i, color, color),
                                  "atta::ui::PhysicsDrawer");
}

} // namespace atta::ui
