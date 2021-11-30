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
		virtual ~PhysicsEngine() = default;
        virtual void step(float dt) = 0;
    };
}

#endif// ATTA_PHYSICS_SYSTEM_PHYSICS_ENGINES_PHYSICS_ENGINE_H
