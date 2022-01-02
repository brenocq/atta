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
    ComponentDescription& TypedComponentRegistry<RigidJointComponent>::getDescription()
    {
        static ComponentDescription desc = 
        {
            "Rigid Joint",
            {
                { AttributeType::INT32, offsetof(RigidJointComponent, bodyA), "bodyA" },
                { AttributeType::INT32, offsetof(RigidJointComponent, bodyB), "bodyB" },
            }
        };

        return desc;
    }
}
