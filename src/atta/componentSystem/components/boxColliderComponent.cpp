//--------------------------------------------------
// Atta Component System
// boxColliderComponent.cpp
// Date: 2021-11-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/boxColliderComponent.h>

namespace atta
{
    template<>
    ComponentDescription& TypedComponentRegistry<BoxColliderComponent>::getDescription()
    {
        static ComponentDescription desc = 
            {
                "Box Collider",
                {
                    { AttributeType::FLOAT32, offsetof(BoxColliderComponent, size), "size", 0.0001f, 2000.0f, 0.01f },
                    { AttributeType::FLOAT32, offsetof(BoxColliderComponent, offset), "offset", -2000.0f, 2000.0f, 0.01f }
                }
            };

        return desc;
    }
}
