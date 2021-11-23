//--------------------------------------------------
// Atta Component System
// transformComponent.cpp
// Date: 2021-11-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/components/transformComponent.h>
#include <atta/componentSystem/components/relationshipComponent.h>

namespace atta
{
    template<>
    ComponentRegistry::Description& TypedComponentRegistry<TransformComponent>::getDescription()
    {
        static ComponentRegistry::Description desc = 
            {
                "Transform",
                {
                    { ComponentRegistry::AttributeType::VECTOR_FLOAT32, offsetof(TransformComponent, position),    "position", -1000.0f, 1000.0f, 0.05f },
                    { ComponentRegistry::AttributeType::QUAT,           offsetof(TransformComponent, orientation), "orientation", -360.0f, 360.0f, 0.5f },
                    { ComponentRegistry::AttributeType::VECTOR_FLOAT32, offsetof(TransformComponent, scale),       "scale", 0.0f, 1000.0f, 0.05f }
                }
            };

        return desc;
    }

    mat4 TransformComponent::getWorldTransform(EntityId entity)
    {
        // For know there is no fast way to the transform component to know to which entity it belongs to,
        // so we receive the EntityId by argument
        mat4 t = getLocalTransform();
        RelationshipComponent* relationship = ComponentManager::getEntityComponent<RelationshipComponent>(entity);

        while(relationship && relationship->getParent() >= 0)
        {
            TransformComponent* ptransform = ComponentManager::getEntityComponent<TransformComponent>(relationship->getParent());
            if(ptransform)
                t = ptransform->getLocalTransform() * t;
            relationship = ComponentManager::getEntityComponent<RelationshipComponent>(relationship->getParent());
        }

        return t;
    }

    mat4 TransformComponent::getLocalTransform()
    {
        mat4 t;
        t.setPosOriScale(position, orientation, scale);
        return t;
    }
}
