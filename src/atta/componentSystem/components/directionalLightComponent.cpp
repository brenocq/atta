//--------------------------------------------------
// Atta Component System
// directionalLightComponent.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/directionalLightComponent.h>

namespace atta
{
    template<>
    ComponentRegistry::Description& TypedComponentRegistry<DirectionalLightComponent>::getDescription()
    {
        static ComponentRegistry::Description desc = 
            {
                "Directional Light",
                {
                    { ComponentRegistry::AttributeType::VECTOR_FLOAT32, offsetof(DirectionalLightComponent, intensity), "intensity", 0.0f, 10.0f }
                }
            };
        return desc;
    }
}
