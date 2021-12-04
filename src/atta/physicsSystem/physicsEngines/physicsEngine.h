//--------------------------------------------------
// Atta Physics System
// physicsEngine.h
// Date: 2021-11-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_SYSTEM_PHYSICS_ENGINES_PHYSICS_ENGINE_H
#define ATTA_PHYSICS_SYSTEM_PHYSICS_ENGINES_PHYSICS_ENGINE_H
#include <atta/componentSystem/componentManager.h>

namespace atta
{
    class PhysicsEngine
    {
    public:
        ///< Available physics engines
        enum Type
        {
            NULL_ENGINE = 0,
            BOX2D_ENGINE,
            PHYSX_ENGINE
        };

        PhysicsEngine(Type type);
        virtual ~PhysicsEngine() = default;
        virtual void start() = 0;
        virtual void step(float dt) = 0;
        virtual void stop() = 0;

        Type getType() const { return _type; }
        bool getRunning() const { return _running; }

    protected:
        Type _type;///< Physics engine type
        bool _running;///< If physics engine is performing simulations
    };
}

#endif// ATTA_PHYSICS_SYSTEM_PHYSICS_ENGINES_PHYSICS_ENGINE_H
