//--------------------------------------------------
// Atta Component System
// transformComponent.h
// Date: 2021-11-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/transformComponent.h>

namespace atta
{
    template<>
    const Component::TypeDescription TypedComponent<TransformComponent>::typeDescription = 
    {
        Component::Type("Transform"),
        {
            { Component::AttributeType::VECTOR_FLOAT32, attributeOffset(&TransformComponent::position),    "position", -1000.0f, 1000.0f, 0.05f },
            { Component::AttributeType::QUAT,           attributeOffset(&TransformComponent::orientation), "orientation", -360.0f, 360.0f, 0.5f },
            { Component::AttributeType::VECTOR_FLOAT32, attributeOffset(&TransformComponent::scale),       "scale", 0.0f, 1000.0f, 0.05f }
        }
    };
}
