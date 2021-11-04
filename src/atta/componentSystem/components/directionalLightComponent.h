//--------------------------------------------------
// Atta Component System
// directionalLightComponent.h
// Date: 2021-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_DIRECTIONAL_LIGHT_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_DIRECTIONAL_LIGHT_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/core/math/math.h>

namespace atta
{
    struct DirectionalLightComponent final : public TypedComponent<DirectionalLightComponent>
    {
        vec3f intensity = vec3f(1.0f, 1.0f, 1.0f);
    };

    template<>
    inline const Component::TypeDescription TypedComponent<DirectionalLightComponent>::typeDescription = 
    {
        "Directional Light",
        {
            { Component::AttributeType::VECTOR_FLOAT32, offsetof(DirectionalLightComponent, intensity), "intensity", 0.0f, 10.0f }
        }
    };
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_DIRECTIONAL_LIGHT_COMPONENT_H
