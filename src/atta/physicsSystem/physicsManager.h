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

namespace atta
{
    class PhysicsManager final
    {
    public:
        enum class Plane2D
        {
            X = 0,
            Y,
            Z
        };

        static PhysicsManager& getInstance();
        static void startUp();
        static void shutDown();
        static void update(float dt);
        static PhysicsEngine::Type getSelectedEngine() { return getInstance()._engine->getType(); }
        static Plane2D getPlane2D() { return getInstance()._plane2D; }
        static void setSelectedEngine(PhysicsEngine::Type type);

    private:
        void startUpImpl();
        void shutDownImpl();
        void updateImpl(float dt);
        void setSelectedEngineImpl(PhysicsEngine::Type type);

        void onSimulationStateChange(Event& event);

        std::shared_ptr<PhysicsEngine> _engine;///< Current physics engine
        Plane2D _plane2D;///< Plane that is used when performing 2D simulations
    };
}

#endif// ATTA_PHYSICS_SYSTEM_PHYSICS_MANAGER_H
