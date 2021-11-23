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
    template<> ComponentRegistry::Description& TypedComponentRegistry<TransformComponent>::getDescription();
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_TRANSFORM_COMPONENT_H
