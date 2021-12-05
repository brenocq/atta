//--------------------------------------------------
// Atta Component System
// sphereColliderComponent.cpp
// Date: 2021-11-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/sphereColliderComponent.h>
#include <atta/componentSystem/components/transformComponent.h>

namespace atta
{
    template<>
    ComponentRegistry::Description& TypedComponentRegistry<SphereColliderComponent>::getDescription()
    {
        static ComponentRegistry::Description desc = 
            {
                "Sphere Collider",
                {
                    { ComponentRegistry::AttributeType::FLOAT32, offsetof(SphereColliderComponent, radius), "radius", 0.0001f, 2000.0f, 0.01f },
                    { ComponentRegistry::AttributeType::FLOAT32, offsetof(SphereColliderComponent, offset), "offset", -2000.0f, 2000.0f,  0.01f  }
                }
            };

        return desc;
    }


    void SphereColliderComponent::getWorldData(EntityId eid, float& worldRadius, vec3& worldPos)
    {
        mat4 transform = TransformComponent::getEntityWorldTransform(eid);
        vec3 pos, scale;
        quat ori;
        transform.getPosOriScale(pos, ori, scale);

        worldRadius = radius*std::max(std::max(scale.x, scale.y), scale.z);
        worldPos = offset+pos;
    }
}
