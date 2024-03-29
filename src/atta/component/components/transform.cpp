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

Transform Transform::getWorldTransform(EntityId entity) {
    // For know there is no fast way to the transform component to know to which entity it belongs to,
    // so we receive the EntityId by argument
    Transform world = *this;

    Relationship* relationship = component::getComponent<Relationship>(entity);
    while (relationship && relationship->getParent() >= 0) {
        Transform* ptransform = component::getComponent<Transform>(relationship->getParent());
        if (ptransform)
            world = (*ptransform) * world;
        relationship = component::getComponent<Relationship>(relationship->getParent());
    }

    return world;
}

void Transform::setWorldTransform(EntityId entity, Transform worldTransform) {
    (*this) = worldTransform;

    // Go up the hierarchy until root to calculate world to local transform
    // TODO maybe need to go top down instead
    Relationship* relationship = component::getComponent<Relationship>(entity);
    while (relationship && relationship->getParent() >= 0) {
        Transform* ptransform = component::getComponent<Transform>(relationship->getParent());
        if (ptransform)
            (*this) = (*this) / *ptransform;
        relationship = component::getComponent<Relationship>(relationship->getParent());
    }
}

mat4 Transform::getWorldTransformMatrix(EntityId entity) {
    Transform t = getWorldTransform(entity);
    mat4 m;
    m.setPosOriScale(t.position, t.orientation, t.scale);
    return m;
}

mat4 Transform::getLocalTransformMatrix() {
    mat4 m;
    m.setPosOriScale(position, orientation, scale);
    return m;
}

Transform Transform::getEntityWorldTransform(EntityId entity) {
    EntityId curr = entity;
    auto t = component::getComponent<Transform>(curr);
    auto r = component::getComponent<Relationship>(curr);

    do {
        if (t)
            return t->getWorldTransform(curr);

        curr = r->getParent();
        if (curr == -1)
            return Transform{};
        else {
            t = component::getComponent<Transform>(curr);
            r = component::getComponent<Relationship>(curr);
        }
    } while (true);
}

Transform Transform::operator*(const Transform& o) const {
    // World = parent * local
    Transform world;

    // Calculate orientation
    world.orientation = orientation * o.orientation;

    // Calculate scale
    vec3 finalScale = o.scale;
    o.orientation.rotateVector(finalScale);
    finalScale *= scale;
    world.scale = finalScale;

    // Calculate position
    vec3 finalPos = scale * o.position;
    orientation.rotateVector(finalPos);
    world.position = position + finalPos;

    return world;
}

Transform Transform::operator/(const Transform& o) const {
    // Local = world / parent
    Transform local;

    quat oriConj = inverse(o.orientation);

    // Calculate orientation
    local.orientation = oriConj * orientation;

    // Calculate scale
    local.scale = scale / o.scale;
    oriConj.rotateVector(local.scale);

    // Calculate position
    local.position = (position - o.position) / o.scale;
    oriConj.rotateVector(local.position);

    return local;
}

} // namespace atta::component
