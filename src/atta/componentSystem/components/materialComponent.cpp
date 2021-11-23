//--------------------------------------------------
// Atta Component System
// materialComponent.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/materialComponent.h>

namespace atta
{
    template<>
    ComponentRegistry::Description& TypedComponentRegistry<MaterialComponent>::getDescription()
    {
        static ComponentRegistry::Description desc = 
            {
                "Material",
                {
                    { ComponentRegistry::AttributeType::VECTOR_FLOAT32, offsetof(MaterialComponent, albedo), "albedo", 0.0f, 1.0f },
                    { ComponentRegistry::AttributeType::FLOAT32, offsetof(MaterialComponent, metallic), "metallic", 0.0f, 1.0f },
                    { ComponentRegistry::AttributeType::FLOAT32, offsetof(MaterialComponent, roughness), "roughness", 0.0f, 1.0f },
                    { ComponentRegistry::AttributeType::FLOAT32, offsetof(MaterialComponent, ao), "ao", 0.0f, 1.0f },
                    { ComponentRegistry::AttributeType::STRINGID, offsetof(MaterialComponent, albedoTexture), "albedoTexture" },
                    { ComponentRegistry::AttributeType::STRINGID, offsetof(MaterialComponent, metallicTexture), "metallicTexture" },
                    { ComponentRegistry::AttributeType::STRINGID, offsetof(MaterialComponent, roughnessTexture), "roughnessTexture" },
                    { ComponentRegistry::AttributeType::STRINGID, offsetof(MaterialComponent, aoTexture), "aoTexture" },
                    { ComponentRegistry::AttributeType::STRINGID, offsetof(MaterialComponent, normalTexture), "normalTexture" }
                }
            };

        return desc;
    }
}
