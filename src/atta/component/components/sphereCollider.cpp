//--------------------------------------------------
// Atta Component Module
// sphereColliderComponent.cpp
// Date: 2021-11-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/sphereColliderComponent.h>
#include <atta/component/components/transformComponent.h>

namespace atta::component {
template <>
ComponentDescription& TypedComponentRegistry<SphereColliderComponent>::getDescription() {
    static ComponentDescription desc = {"Sphere Collider",
                                        {{AttributeType::FLOAT32, offsetof(SphereColliderComponent, radius), "radius", 0.0001f, 2000.0f, 0.01f},
                                         {AttributeType::FLOAT32, offsetof(SphereColliderComponent, offset), "offset", -2000.0f, 2000.0f, 0.01f}}};

    return desc;
}

void SphereColliderComponent::getWorldData(EntityId eid, float& worldRadius, vec3& worldPos) {
    mat4 transform = TransformComponent::getEntityWorldTransform(eid);
    vec3 pos, scale;
    quat ori;
    transform.getPosOriScale(pos, ori, scale);

    worldRadius = radius * std::max(std::max(scale.x, scale.y), scale.z);
    worldPos = offset + pos;
}
} // namespace atta::component
