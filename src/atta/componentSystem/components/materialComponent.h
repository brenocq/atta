//--------------------------------------------------
// Atta Component System
// materialComponent.h
// Date: 2021-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_MATERIAL_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_MATERIAL_COMPONENT_H
#include <atta/core/math/math.h>

namespace atta
{
    struct MaterialComponent
    {
        vec3 albedo;
        float metallic;
        float roughness;
        float ao;
        StringId albedoTexture;
        StringId metallicTexture;
        StringId roughnessTexture;
        StringId aoTexture;
        StringId normalTexture;

        MaterialComponent(): 
            albedo(vec3(1.0f, 0.0f, 1.0f)), metallic(0.0f), roughness(0.5f),
            ao(1.0f), albedoTexture(StringId("Empty texture")), metallicTexture(StringId("Empty texture")),
            roughnessTexture(StringId("Empty texture")), aoTexture(StringId("Empty texture")),
            normalTexture(StringId("Empty texture"))
        {}
    };
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_MATERIAL_COMPONENT_H
