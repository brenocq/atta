//--------------------------------------------------
// Atta Physics Module
// manager.cpp
// Date: 2021-11-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/event/events/simulationStart.h>
#include <atta/event/events/simulationStop.h>
#include <atta/event/manager.h>
#include <atta/physics/manager.h>

#include <atta/physics/physicsEngines/box2DEngine.h>
#include <atta/physics/physicsEngines/bulletEngine.h>
#include <atta/physics/physicsEngines/nullEngine.h>

namespace atta::physics {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::startUp() { getInstance().startUpImpl(); }
void Manager::startUpImpl() {
    event::Manager::subscribe<event::SimulationStart>(BIND_EVENT_FUNC(Manager::onSimulationStateChange));
    event::Manager::subscribe<event::SimulationStop>(BIND_EVENT_FUNC(Manager::onSimulationStateChange));

    _engine = std::make_shared<Box2DEngine>();
    _plane2D = Plane2D::Z;
}

void Manager::shutDown() { getInstance().shutDownImpl(); }
void Manager::shutDownImpl() {}

void Manager::update(float dt) { getInstance().updateImpl(dt); }
void Manager::updateImpl(float dt) {
    DASSERT(_engine != nullptr, "Physics engine must not be nullptr");
    _engine->step(dt);
}

void Manager::setSelectedEngine(PhysicsEngine::Type type) { getInstance().setSelectedEngineImpl(type); }
void Manager::setSelectedEngineImpl(PhysicsEngine::Type type) {
    DASSERT(_engine != nullptr, "Physics engine must not be nullptr");
    if (type == _engine->getType())
        return;

    bool isRunning = _engine->getRunning();
    _engine.reset();

    switch (type) {
    case PhysicsEngine::NULL_ENGINE:
        _engine = std::make_shared<NullEngine>();
        break;
    case PhysicsEngine::BOX2D_ENGINE:
        _engine = std::make_shared<Box2DEngine>();
        break;
    case PhysicsEngine::BULLET_ENGINE:
        _engine = std::make_shared<BulletEngine>();
        break;
    default:
        LOG_WARN("physics::Manager", "Trying to select unknown physics engine");
    }

    if (isRunning)
        _engine->start();
}

std::vector<component::EntityId> Manager::getEntityCollisions(component::EntityId eid) { return getInstance()._engine->getEntityCollisions(eid); }

std::vector<component::EntityId> Manager::rayCast(vec3 begin, vec3 end, bool onlyFirst) {
    return getInstance()._engine->rayCast(begin, end, onlyFirst);
}

bool Manager::areColliding(component::EntityId eid0, component::EntityId eid1) { return getInstance()._engine->areColliding(eid0, eid1); }

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

} // namespace atta::physics
