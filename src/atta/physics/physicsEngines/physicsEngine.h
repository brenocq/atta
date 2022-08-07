//--------------------------------------------------
// Atta Physics Module
// physicsEngine.h
// Date: 2021-11-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_PHYSICS_ENGINES_PHYSICS_ENGINE_H
#define ATTA_PHYSICS_PHYSICS_ENGINES_PHYSICS_ENGINE_H
#include <atta/component/componentManager.h>
#include <atta/core/math/math.h>

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
            BULLET_ENGINE
        };

        PhysicsEngine(Type type);
        virtual ~PhysicsEngine() = default;
        virtual void start() = 0;
        virtual void step(float dt) = 0;
        virtual void stop() = 0;

        virtual std::vector<EntityId> getEntityCollisions(EntityId eid) { return {}; };
        virtual std::vector<EntityId> rayCast(vec3 begin, vec3 end, bool onlyFirst = true) { return {}; };
        virtual bool areColliding(EntityId eid0, EntityId eid1) { return false; };

        Type getType() const { return _type; }
        bool getRunning() const { return _running; }

    protected:
        Type _type;///< Physics engine type
        bool _running;///< If physics engine is performing simulations
    };
}

#endif// ATTA_PHYSICS_PHYSICS_ENGINES_PHYSICS_ENGINE_H
