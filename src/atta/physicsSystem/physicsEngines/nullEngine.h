//--------------------------------------------------
// Atta Physics System
// nullEngine.h
// Date: 2021-12-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_SYSTEM_PHYSICS_ENGINES_NULL_ENGINE_H
#define ATTA_PHYSICS_SYSTEM_PHYSICS_ENGINES_NULL_ENGINE_H
#include <atta/physicsSystem/physicsEngines/physicsEngine.h>

namespace atta
{
    class NullEngine : public PhysicsEngine
    {
    public:
		NullEngine(): PhysicsEngine(PhysicsEngine::NULL_ENGINE) {}

        void start() override {}
        void step(float dt) override {}
        void stop() override {}
    };
}

#endif// ATTA_PHYSICS_SYSTEM_PHYSICS_ENGINES_NULL_ENGINE_H
