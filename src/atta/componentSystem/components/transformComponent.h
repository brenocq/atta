//--------------------------------------------------
// Atta Component System
// transformComponent.h
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_TRANSFORM_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_TRANSFORM_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/componentSystem/componentRegistry.h>
#include <atta/core/math/math.h>

namespace atta
{
    struct TransformComponent final : public Component
    {
        vec3f position = vec3();
        quat orientation = quat();
        vec3f scale = vec3(1,1,1);

        mat4 getWorldTransform(EntityId entity);
        mat4 getLocalTransform();
    };
    ATTA_REGISTER_COMPONENT(TransformComponent)

    template<>
    inline ComponentRegistry::Description TypedComponentRegistry<TransformComponent>::description = 
    {
        "Transform",
        {
            { ComponentRegistry::AttributeType::VECTOR_FLOAT32, offsetof(TransformComponent, position),    "position", -1000.0f, 1000.0f, 0.05f },
            { ComponentRegistry::AttributeType::QUAT,           offsetof(TransformComponent, orientation), "orientation", -360.0f, 360.0f, 0.5f },
            { ComponentRegistry::AttributeType::VECTOR_FLOAT32, offsetof(TransformComponent, scale),       "scale", 0.0f, 1000.0f, 0.05f }
        }
    };
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_TRANSFORM_COMPONENT_H
