//--------------------------------------------------
// Atta Physics Module
// manager.cpp
// Date: 2021-11-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/physics/interface.h>

#include <atta/physics/engines/box2DEngine.h>
#include <atta/physics/engines/bulletEngine.h>
#include <atta/physics/engines/noneEngine.h>

#include <atta/event/events/createComponent.h>
#include <atta/event/events/deleteComponent.h>
#include <atta/event/events/simulationStart.h>
#include <atta/event/events/simulationStop.h>
#include <atta/event/interface.h>

#include <atta/component/components/boxCollider.h>
#include <atta/component/components/boxCollider2D.h>
#include <atta/component/components/circleCollider2D.h>
#include <atta/component/components/cylinderCollider.h>
#include <atta/component/components/rigidBody.h>
#include <atta/component/components/rigidBody2D.h>
#include <atta/component/components/sphereCollider.h>

namespace atta::physics {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::startUpImpl() {
    event::subscribe<event::SimulationStart>(BIND_EVENT_FUNC(Manager::onSimulationStateChange));
    event::subscribe<event::SimulationStop>(BIND_EVENT_FUNC(Manager::onSimulationStateChange));
    event::subscribe<event::CreateComponent>(BIND_EVENT_FUNC(Manager::onComponentChange));
    event::subscribe<event::DeleteComponent>(BIND_EVENT_FUNC(Manager::onComponentChange));

    _noneEngine = std::make_shared<NoneEngine>();
    _box2DEngine = std::make_shared<Box2DEngine>();
    _bulletEngine = std::make_shared<BulletEngine>();
    _engine = _bulletEngine;

    _plane2D = Plane2D::Z;
    _gravity = vec3(0.0f, 0.0f, -9.81f);
    _showColliders = true;
    _showContacts = false;
}

void Manager::shutDownImpl() {}

void Manager::updateImpl(float dt) {
    DASSERT(_engine != nullptr, "Physics engine must not be nullptr");
    _engine->step(dt);
}

void Manager::setEngineTypeImpl(Engine::Type type) {
    DASSERT(_engine != nullptr, "Physics engine must not be nullptr");
    if (type == _engine->getType())
        return;

    bool isRunning = _engine->getRunning();
    _engine.reset();

    switch (type) {
        case Engine::NONE:
            _engine = _noneEngine;
            break;
        case Engine::BOX2D:
            _engine = _box2DEngine;
            break;
        case Engine::BULLET:
            _engine = _bulletEngine;
            break;
        default:
            LOG_WARN("physics::Manager", "Trying to select unknown physics engine");
    }

    if (isRunning)
        _engine->start();
}

void Manager::setPlane2DImpl(Plane2D plane2D) {
    _plane2D = plane2D;
    if (_engine->getType() == Engine::BOX2D) {
        // TODO update box2d plane
    }
}

void Manager::setGravityImpl(vec3 gravity) {
    _gravity = gravity;
    _engine->updateGravity();
}

void Manager::onSimulationStateChange(event::Event& event) {
    switch (event.getType()) {
        case event::SimulationStart::type:
            _engine->start();
            break;
        case event::SimulationStop::type:
            _engine->stop();
            break;
        default:
            LOG_WARN("physics::Manager", "Unknown simulation event");
    }
}

bool is2DPhysicsColliderComponent(cmp::ComponentId cmpId) {
    return cmpId == cmp::getId<cmp::BoxCollider2D>() || cmpId == cmp::getId<cmp::CircleCollider2D>();
}

bool is3DPhysicsColliderComponent(cmp::ComponentId cmpId) {
    return cmpId == cmp::getId<cmp::BoxCollider>() || cmpId == cmp::getId<cmp::SphereCollider>() || cmpId == cmp::getId<cmp::CylinderCollider>();
}

void Manager::onComponentChange(event::Event& event) {
    // Handle dynamically adding/removing colliders and rigid body during simulation
    switch (event.getType()) {
        case event::CreateComponent::type: {
            event::CreateComponent& e = reinterpret_cast<event::CreateComponent&>(event);
            if (_engine->getType() == Engine::BOX2D && _engine->getRunning()) {
                if (e.componentId == cmp::getId<cmp::RigidBody2D>())
                    _engine->createRigidBody(e.entityId);
                if (is2DPhysicsColliderComponent(e.componentId))
                    _engine->createColliders(e.entityId);
            }
            if (is3DPhysicsColliderComponent(e.componentId)) {
                // TODO
            }
            break;
        }
        case event::DeleteComponent::type: {
            event::DeleteComponent& e = reinterpret_cast<event::DeleteComponent&>(event);
            if (_engine->getType() == Engine::BOX2D && _engine->getRunning()) {
                if (e.componentId == cmp::getId<cmp::RigidBody2D>())
                    _engine->deleteRigidBody(e.entityId);
                if (is2DPhysicsColliderComponent(e.componentId))
                    _engine->deleteColliders(e.entityId);
            }
            if (is3DPhysicsColliderComponent(e.componentId)) {
                // TODO
            }
            break;
        }
        default:
            LOG_WARN("physics::Manager", "Unknown simulation event");
    }
}

} // namespace atta::physics
