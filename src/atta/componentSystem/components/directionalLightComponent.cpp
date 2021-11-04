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
            { Component::AttributeType::VECTOR_FLOAT32, attributeOffset(&DirectionalLightComponent::intensity), "intensity", 0.0f, 10.0f }
        }
    };
}
