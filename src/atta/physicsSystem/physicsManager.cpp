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
	}

    void PhysicsManager::shutDown() { getInstance().shutDownImpl(); }
	void PhysicsManager::shutDownImpl()
	{

	}

    void PhysicsManager::update(float dt) { getInstance().updateImpl(dt); }
    void PhysicsManager::updateImpl(float dt)
	{
		DASSERT(_physicsEngine != nullptr, "Physics engine must not be nullptr while the simulation is running");
		_physicsEngine->step(dt);
	}

    void PhysicsManager::onSimulationStateChange(Event& event)
	{
        switch(event.getType())
        {
            case SimulationStartEvent::type:
                {
					LOG_DEBUG("PhysicsManager", "Start event");

					_physicsEngine = std::make_shared<Box2DEngine>();

                    break;
                }
            case SimulationStopEvent::type:
                {
					LOG_DEBUG("PhysicsManager", "Stop event");
					_physicsEngine.reset();
                    break;
                }
            default:
                {
                    LOG_WARN("PhysicsManager", "Unknown simulation event");
                }
        }
	}
}
