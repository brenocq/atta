//--------------------------------------------------
// Atta Component Module
// pointLightComponent.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/pointLightComponent.h>

namespace atta
{
    template<>
    ComponentDescription& TypedComponentRegistry<PointLightComponent>::getDescription()
    {
        static ComponentDescription desc = 
            {
                "Point Light",
                {
                    { AttributeType::VECTOR_FLOAT32, offsetof(PointLightComponent, intensity), "intensity", 0.0f, 10.0f }
                }
            };

        return desc;
    }
}
