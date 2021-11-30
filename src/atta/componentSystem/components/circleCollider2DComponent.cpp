//--------------------------------------------------
// Atta Component System
// circleCollider2DComponent.cpp
// Date: 2021-11-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/circleCollider2DComponent.h>

namespace atta
{
    template<>
    ComponentRegistry::Description& TypedComponentRegistry<CircleCollider2DComponent>::getDescription()
    {
        static ComponentRegistry::Description desc = 
            {
                "CircleCollider2D",
                {
                    { ComponentRegistry::AttributeType::FLOAT32, offsetof(CircleCollider2DComponent, radius), "radius", 0.0001f, 2000.0f, 0.01f },
                    { ComponentRegistry::AttributeType::FLOAT32, offsetof(CircleCollider2DComponent, offset), "offset", -2000.0f, 2000.0f,  0.01f  }
                }
            };

        return desc;
    }
}
