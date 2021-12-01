//--------------------------------------------------
// Atta Physics System
// physicsManager.h
// Date: 2021-11-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_SYSTEM_PHYSICS_MANAGER_H
#define ATTA_PHYSICS_SYSTEM_PHYSICS_MANAGER_H
#include <atta/eventSystem/event.h>
#include <atta/physicsSystem/physicsEngines/physicsEngine.h>

class b2World;
namespace atta
{
    class PhysicsManager final
    {
    public:
        static PhysicsManager& getInstance();
        static void startUp();
        static void shutDown();
        static void update(float dt);
		static PhysicsEngine::Type getSelectedEngine() { return getInstance()._engine->getType(); }
		static void setSelectedEngine(PhysicsEngine::Type type);

    private:
        void startUpImpl();
        void shutDownImpl();
        void updateImpl(float dt);
		void setSelectedEngineImpl(PhysicsEngine::Type type);

    	void onSimulationStateChange(Event& event);

		std::shared_ptr<PhysicsEngine> _engine;
		bool _running;
    };
}

#endif// ATTA_PHYSICS_SYSTEM_PHYSICS_MANAGER_H
