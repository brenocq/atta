//--------------------------------------------------
// Atta Physics Module
// manager.cpp
// Date: 2021-11-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/physics/manager.h>

#include <atta/physics/physicsEngines/box2DEngine.h>
#include <atta/physics/physicsEngines/bulletEngine.h>
#include <atta/physics/physicsEngines/nullEngine.h>

#include <atta/event/events/simulationStart.h>
#include <atta/event/events/simulationStop.h>
#include <atta/event/interface.h>

namespace atta::physics {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::startUpImpl() {
    event::subscribe<event::SimulationStart>(BIND_EVENT_FUNC(Manager::onSimulationStateChange));
    event::subscribe<event::SimulationStop>(BIND_EVENT_FUNC(Manager::onSimulationStateChange));

    _engine = std::make_shared<Box2DEngine>();
    _plane2D = Plane2D::Z;
}

void Manager::shutDownImpl() {}

void Manager::updateImpl(float dt) {
    DASSERT(_engine != nullptr, "Physics engine must not be nullptr");
    _engine->step(dt);
}

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
