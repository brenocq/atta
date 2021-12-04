//--------------------------------------------------
// Atta Component System
// rigidJointComponent.cpp
// Date: 2021-12-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/rigidJointComponent.h>

namespace atta
{
    template<>
    ComponentRegistry::Description& TypedComponentRegistry<RigidJointComponent>::getDescription()
    {
        static ComponentRegistry::Description desc = 
        {
            "Rigid Joint",
            {
                { ComponentRegistry::AttributeType::INT32, offsetof(RigidJointComponent, bodyA), "bodyA" },
                { ComponentRegistry::AttributeType::INT32, offsetof(RigidJointComponent, bodyB), "bodyB" },
            }
        };

        return desc;
    }
}
