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
		_engine = std::make_shared<NullEngine>();
		_running = false;
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

		if(_running)
			_engine->stop();

		switch(type)
		{
			case PhysicsEngine::NULL_ENGINE:
				_engine = std::make_shared<NullEngine>();
				break;
			case PhysicsEngine::BOX2D_ENGINE:
				_engine = std::make_shared<Box2DEngine>();
				break;
			case PhysicsEngine::PHYSX_ENGINE:
				LOG_WARN("PhysicsManager", "PhysX engine was not implemented yet");
				_engine = std::make_shared<NullEngine>();
				break;
			default:
				LOG_WARN("PhysicsManager", "Trying to select unknown physics engine");
		}

		if(_running)
			_engine->start();
	}

    void PhysicsManager::onSimulationStateChange(Event& event)
	{
        switch(event.getType())
        {
            case SimulationStartEvent::type:
				_engine->start();
				_running = true;
				break;
            case SimulationStopEvent::type:
				_engine->stop();
				_running = false;
				break;
            default:
                LOG_WARN("PhysicsManager", "Unknown simulation event");
        }
	}
}
