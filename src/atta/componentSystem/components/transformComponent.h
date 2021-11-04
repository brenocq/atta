//--------------------------------------------------
// Atta Component System
// transformComponent.h
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_TRANSFORM_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_TRANSFORM_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/core/math/math.h>

namespace atta
{
    struct TransformComponent final : public TypedComponent<TransformComponent>
    {
        vec3f position = vec3();
        quat orientation = quat();
        vec3f scale = vec3(1,1,1);
    };

    template<>
    inline const Component::TypeDescription TypedComponent<TransformComponent>::typeDescription = 
    {
        "Transform",
        {
            { Component::AttributeType::VECTOR_FLOAT32, offsetof(TransformComponent, position),    "position", -1000.0f, 1000.0f, 0.05f },
            { Component::AttributeType::QUAT,           offsetof(TransformComponent, orientation), "orientation", -360.0f, 360.0f, 0.5f },
            { Component::AttributeType::VECTOR_FLOAT32, offsetof(TransformComponent, scale),       "scale", 0.0f, 1000.0f, 0.05f }
        }
    };
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_TRANSFORM_COMPONENT_H
