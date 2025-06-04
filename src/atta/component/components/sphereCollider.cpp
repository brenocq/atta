//--------------------------------------------------
// Atta Component Module
// sphereCollider.cpp
// Date: 2021-11-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/sphereCollider.h>
#include <atta/component/components/transform.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<SphereCollider>::getDescription() {
    static ComponentDescription desc = {
        "Sphere Collider",
        {
            {AttributeType::FLOAT32, offsetof(SphereCollider, radius), "radius", 0.0001f, 2000.0f, 0.01f},
            {AttributeType::VECTOR_FLOAT32, offsetof(SphereCollider, offset), "offset", -2000.0f, 2000.0f, 0.01f},
        },
    };

    return desc;
}

void SphereCollider::getWorldData(EntityId eid, float& worldRadius, vec3& worldPos) {
    Transform wt = Transform::getEntityWorldTransform(eid);
    vec3 pos = wt.position;
    vec3 scale = wt.scale;
    quat ori = wt.orientation;

    worldRadius = radius * std::max(std::max(scale.x, scale.y), scale.z);
    worldPos = offset + pos;
}

} // namespace atta::component
