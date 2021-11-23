//--------------------------------------------------
// Atta Component System
// pointLightComponent.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/pointLightComponent.h>

namespace atta
{
    template<>
    ComponentRegistry::Description& TypedComponentRegistry<PointLightComponent>::getDescription()
    {
        static ComponentRegistry::Description desc = 
            {
                "Point Light",
                {
                    { ComponentRegistry::AttributeType::VECTOR_FLOAT32, offsetof(PointLightComponent, intensity), "intensity", 0.0f, 10.0f }
                }
            };

        return desc;
    }
}
