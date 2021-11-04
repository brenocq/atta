//--------------------------------------------------
// Atta Component System
// pointLightComponent.h
// Date: 2021-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_POINT_LIGHT_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_POINT_LIGHT_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/core/math/math.h>

namespace atta
{
    struct PointLightComponent final : public TypedComponent<PointLightComponent>
    {
        vec3f intensity = vec3(1.0f, 1.0f, 1.0f);
    };

    template<>
    inline const Component::TypeDescription TypedComponent<PointLightComponent>::typeDescription = 
    {
        "Point Light",
        {
            { Component::AttributeType::VECTOR_FLOAT32, offsetof(PointLightComponent, intensity), "intensity", 0.0f, 10.0f }
        }
    };
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_POINT_LIGHT_COMPONENT_H
