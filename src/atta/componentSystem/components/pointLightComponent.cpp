//--------------------------------------------------
// Atta Component System
// pointLightComponent.cpp
// Date: 2021-11-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/pointLightComponent.h>

namespace atta
{
    template<>
    const Component::TypeDescription TypedComponent<PointLightComponent>::typeDescription = 
    {
        Component::Type("Point Light"),
        {
            { Component::AttributeType::VECTOR_FLOAT32, attributeOffset(&PointLightComponent::intensity), "intensity", .min = 0.0f, .max = 10.0f }
        }
    };
}
