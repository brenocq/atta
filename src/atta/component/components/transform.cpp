//--------------------------------------------------
// Atta Component Module
// transform.cpp
// Date: 2021-11-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/relationship.h>
#include <atta/component/components/transform.h>
#include <atta/component/interface.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<Transform>::getDescription() {
    static ComponentDescription desc = {"Transform",
                                        {{AttributeType::VECTOR_FLOAT32, offsetof(Transform, position), "position", -1000.0f, 1000.0f, 0.05f},
                                         {AttributeType::QUAT, offsetof(Transform, orientation), "orientation", -360.0f, 360.0f, 0.5f},
                                         {AttributeType::VECTOR_FLOAT32, offsetof(Transform, scale), "scale", 0.0f, 1000.0f, 0.05f}}};

    return desc;
}

mat4 Transform::getWorldTransform(EntityId entity) {
    // For know there is no fast way to the transform component to know to which entity it belongs to,
    // so we receive the EntityId by argument
    mat4 t = getLocalTransform();
    Relationship* relationship = component::getComponent<Relationship>(entity);

    while (relationship && relationship->getParent() >= 0) {
        Transform* ptransform = component::getComponent<Transform>(relationship->getParent());
        if (ptransform)
            t = ptransform->getLocalTransform() * t;
        relationship = component::getComponent<Relationship>(relationship->getParent());
    }

    return t;
}

mat4 Transform::getLocalTransform() {
    mat4 t;
    t.setPosOriScale(position, orientation, scale);
    return t;
}

void Transform::setWorldTransform(EntityId entity, mat4 worldTransform) {
    // If 2 is parent of 1, that is parent of 0
    // LX is the local transform of X
    // We have: W0 = L2*L1*L0
    // We want: worldToLocal*W0 = L0
    // Thus:    worldToLocal = (L2*L1)^(-1)

    mat4 worldToLocal = mat4(1.0f);

    // Go up the hierarchy until root to calculate world to local transform
    Relationship* relationship = component::getComponent<Relationship>(entity);
    while (relationship && relationship->getParent() >= 0) {
        Transform* ptransform = component::getComponent<Transform>(relationship->getParent());
        if (ptransform)
            worldToLocal = ptransform->getLocalTransform() * worldToLocal;
        relationship = component::getComponent<Relationship>(relationship->getParent());
    }
    worldToLocal.invert();

    mat4 local = worldToLocal * worldTransform;
    local.getPosOriScale(position, orientation, scale);
}

mat4 Transform::getEntityWorldTransform(EntityId entity) {
    EntityId curr = entity;
    auto t = component::getComponent<Transform>(curr);
    auto r = component::getComponent<Relationship>(curr);

    do {
        if (t)
            return t->getWorldTransform(curr);

        curr = r->getParent();
        if (curr == -1)
            return mat4(1.0f);
        else {
            t = component::getComponent<Transform>(curr);
            r = component::getComponent<Relationship>(curr);
        }
    } while (true);
}

} // namespace atta::component
