//--------------------------------------------------
// Atta Component System
// directionalLightComponent.cpp
// Date: 2021-11-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/directionalLightComponent.h>

namespace atta
{
    template<>
    const Component::TypeDescription TypedComponent<DirectionalLightComponent>::typeDescription = 
    {
        Component::Type("Directional Light"),
        {
            { Component::AttributeType::VECTOR_FLOAT32, attributeOffset(&DirectionalLightComponent::intensity), "intensity", .min = 0.0f, .max = 10.0f }
        }
    };
}
