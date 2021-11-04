//--------------------------------------------------
// Atta Component System
// materialComponent.cpp
// Date: 2021-11-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/materialComponent.h>

namespace atta
{
    template<>
    const Component::TypeDescription TypedComponent<MaterialComponent>::typeDescription = 
    {
        Component::Type("Material"),
        {
            { Component::AttributeType::VECTOR_FLOAT32, attributeOffset(&MaterialComponent::albedo), "albedo", .min=0.0f, .max=1.0f },
            { Component::AttributeType::FLOAT32, attributeOffset(&MaterialComponent::metallic), "metallic", .min=0.0f, .max=1.0f },
            { Component::AttributeType::FLOAT32, attributeOffset(&MaterialComponent::roughness), "roughness", .min=0.0f, .max=1.0f },
            { Component::AttributeType::FLOAT32, attributeOffset(&MaterialComponent::ao), "ao", .min=0.0f, .max=1.0f },
            { Component::AttributeType::STRINGID, attributeOffset(&MaterialComponent::albedoTexture), "albedoTexture" },
            { Component::AttributeType::STRINGID, attributeOffset(&MaterialComponent::metallicTexture), "metallicTexture" },
            { Component::AttributeType::STRINGID, attributeOffset(&MaterialComponent::roughnessTexture), "roughnessTexture" },
            { Component::AttributeType::STRINGID, attributeOffset(&MaterialComponent::aoTexture), "aoTexture" },
            { Component::AttributeType::STRINGID, attributeOffset(&MaterialComponent::normalTexture), "normalTexture" }
        }
    };
}
