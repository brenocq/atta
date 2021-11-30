//--------------------------------------------------
// Atta Component System
// boxCollider2DComponent.cpp
// Date: 2021-11-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/boxCollider2DComponent.h>

namespace atta
{
    template<>
    ComponentRegistry::Description& TypedComponentRegistry<BoxCollider2DComponent>::getDescription()
    {
        static ComponentRegistry::Description desc = 
            {
                "BoxCollider2D",
                {
                    { ComponentRegistry::AttributeType::FLOAT32, offsetof(BoxCollider2DComponent, size), "size", 0.0001f, 2000.0f, 0.01f },
                    { ComponentRegistry::AttributeType::FLOAT32, offsetof(BoxCollider2DComponent, offset), "offset", -2000.0f, 2000.0f,  0.01f  }
                }
            };

        return desc;
    }
}
