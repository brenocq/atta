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
