//--------------------------------------------------
// Atta Physics System
// physicsManager.cpp
// Date: 2021-11-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/physicsSystem/physicsManager.h>
#include <atta/eventSystem/eventManager.h>
#include <atta/eventSystem/events/simulationStartEvent.h>
#include <atta/eventSystem/events/simulationStopEvent.h>

#include <atta/physicsSystem/physicsEngines/nullEngine.h>
#include <atta/physicsSystem/physicsEngines/box2DEngine.h>
#include <atta/physicsSystem/physicsEngines/bulletEngine.h>

namespace atta
{
    PhysicsManager& PhysicsManager::getInstance()
    {
        static PhysicsManager instance;
        return instance;
    }

    void PhysicsManager::startUp() { getInstance().startUpImpl(); }
    void PhysicsManager::startUpImpl()
    {
        EventManager::subscribe<SimulationStartEvent>(BIND_EVENT_FUNC(PhysicsManager::onSimulationStateChange));
        EventManager::subscribe<SimulationStopEvent>(BIND_EVENT_FUNC(PhysicsManager::onSimulationStateChange));

        _engine = std::make_shared<Box2DEngine>();
        _plane2D = Plane2D::Z;
    }

    void PhysicsManager::shutDown() { getInstance().shutDownImpl(); }
    void PhysicsManager::shutDownImpl()
    {

    }

    void PhysicsManager::update(float dt) { getInstance().updateImpl(dt); }
    void PhysicsManager::updateImpl(float dt)
    {
        DASSERT(_engine != nullptr, "Physics engine must not be nullptr");
        _engine->step(dt);
    }

    void PhysicsManager::setSelectedEngine(PhysicsEngine::Type type) { getInstance().setSelectedEngineImpl(type); }
    void PhysicsManager::setSelectedEngineImpl(PhysicsEngine::Type type)
    {
        DASSERT(_engine != nullptr, "Physics engine must not be nullptr");
        if(type == _engine->getType()) return;

        bool isRunning = _engine->getRunning();
        _engine.reset();

        switch(type)
        {
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
                LOG_WARN("PhysicsManager", "Trying to select unknown physics engine");
        }

        if(isRunning)
            _engine->start();
    }

    std::vector<EntityId> PhysicsManager::getEntityCollisions(EntityId eid)
    { 
        return getInstance()._engine->getEntityCollisions(eid);
    }

    std::vector<EntityId> PhysicsManager::rayCast(vec3 begin, vec3 end, bool onlyFirst)
    { 
        return getInstance()._engine->rayCast(begin, end, onlyFirst);
    }

    bool PhysicsManager::areColliding(EntityId eid0, EntityId eid1)
    { 
        return getInstance()._engine->areColliding(eid0, eid1);
    }

    void PhysicsManager::onSimulationStateChange(Event& event)
    {
        switch(event.getType())
        {
            case SimulationStartEvent::type:
                _engine->start();
                break;
            case SimulationStopEvent::type:
                _engine->stop();
                break;
            default:
                LOG_WARN("PhysicsManager", "Unknown simulation event");
        }
    }
}
