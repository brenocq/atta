//--------------------------------------------------
// Atta Physics System
// physicsManager.cpp
// Date: 2021-11-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/physicsSystem/physicsManager.h>

namespace atta
{
	PhysicsManager& PhysicsManager::getInstance()
	{
        static PhysicsManager instance;
        return instance;
	}

	void PhysicsManager::startUp()
	{

	}

	void PhysicsManager::shutDown()
	{

	}
}
