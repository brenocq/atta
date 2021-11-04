//--------------------------------------------------
// Atta Component System
// materialComponent.h
// Date: 2021-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_MATERIAL_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_MATERIAL_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/core/math/math.h>

namespace atta
{
    struct MaterialComponent final : public TypedComponent<MaterialComponent>
    {
        vec3f albedo = vec3(1.0f, 0.0f, 1.0f);
        float metallic = 0.0f;
        float roughness = 0.5f;
        float ao = 1.0f;
        StringId albedoTexture = StringId("Empty texture");
        StringId metallicTexture = StringId("Empty texture");
        StringId roughnessTexture = StringId("Empty texture");
        StringId aoTexture = StringId("Empty texture");
        StringId normalTexture = StringId("Empty texture");
    };

    template<>
    inline const Component::TypeDescription TypedComponent<MaterialComponent>::typeDescription = 
    {
        "Material",
        {
            { Component::AttributeType::VECTOR_FLOAT32, offsetof(MaterialComponent, albedo), "albedo", 0.0f, 1.0f },
            { Component::AttributeType::FLOAT32, offsetof(MaterialComponent, metallic), "metallic", 0.0f, 1.0f },
            { Component::AttributeType::FLOAT32, offsetof(MaterialComponent, roughness), "roughness", 0.0f, 1.0f },
            { Component::AttributeType::FLOAT32, offsetof(MaterialComponent, ao), "ao", 0.0f, 1.0f },
            { Component::AttributeType::STRINGID, offsetof(MaterialComponent, albedoTexture), "albedoTexture" },
            { Component::AttributeType::STRINGID, offsetof(MaterialComponent, metallicTexture), "metallicTexture" },
            { Component::AttributeType::STRINGID, offsetof(MaterialComponent, roughnessTexture), "roughnessTexture" },
            { Component::AttributeType::STRINGID, offsetof(MaterialComponent, aoTexture), "aoTexture" },
            { Component::AttributeType::STRINGID, offsetof(MaterialComponent, normalTexture), "normalTexture" }
        }
    };
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_MATERIAL_COMPONENT_H
