//--------------------------------------------------
// Atta Physics System
// physicsManager.h
// Date: 2021-11-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_SYSTEM_PHYSICS_MANAGER_H
#define ATTA_PHYSICS_SYSTEM_PHYSICS_MANAGER_H

namespace atta
{
    class PhysicsManager final
    {
    public:
        static PhysicsManager& getInstance();
        static void startUp();
        static void shutDown();

    private:
    };
}

#endif// ATTA_PHYSICS_SYSTEM_PHYSICS_MANAGER_H
