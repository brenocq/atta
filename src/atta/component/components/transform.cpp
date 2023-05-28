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
ComponentDescription& TypedRegistry<Transform>::getDescription() {
    static ComponentDescription desc = {"Transform",
                                        {{AttributeType::VECTOR_FLOAT32, offsetof(Transform, position), "position", -1000.0f, 1000.0f, 0.05f},
                                         {AttributeType::QUAT, offsetof(Transform, orientation), "orientation", -360.0f, 360.0f, 0.5f},
                                         {AttributeType::VECTOR_FLOAT32, offsetof(Transform, scale), "scale", 0.0f, 1000.0f, 0.05f}}};

    return desc;
}

ATTA_CPU_GPU Transform Transform::getWorldTransform(Entity entity) {
    // For know there is no fast way to the transform component to know to which entity it belongs to,
    // so we receive the Entity by argument
    Transform world = *this;

    Relationship* relationship = entity.get<Relationship>();
    while (relationship && relationship->getParent() >= 0) {
        Transform* ptransform = relationship->getParent().get<Transform>();
        if (ptransform)
            world = (*ptransform) * world;
        relationship = relationship->getParent().get<Relationship>();
    }

    return world;
}

ATTA_CPU_GPU void Transform::setWorldTransform(Entity entity, Transform worldTransform) {
    (*this) = worldTransform;

    // Go up the hierarchy until root to calculate world to local transform
    // TODO maybe need to go top down instead
    Relationship* relationship = entity.get<Relationship>();
    while (relationship && relationship->getParent() >= 0) {
        Transform* ptransform = relationship->getParent().get<Transform>();
        if (ptransform)
            (*this) = (*this) / *ptransform;
        relationship = relationship->getParent().get<Relationship>();
    }
}

ATTA_CPU_GPU mat4 Transform::getWorldTransformMatrix(Entity entity) {
    Transform t = getWorldTransform(entity);
    mat4 m;
    m.setPosOriScale(t.position, t.orientation, t.scale);
    return m;
}

ATTA_CPU_GPU mat4 Transform::getLocalTransformMatrix() {
    mat4 m;
    m.setPosOriScale(position, orientation, scale);
    return m;
}

ATTA_CPU_GPU Transform Transform::getEntityWorldTransform(Entity entity) {
    Entity curr = entity;
    auto t = curr.get<Transform>();
    auto r = curr.get<Relationship>();

    do {
        if (t)
            return t->getWorldTransform(curr);

        curr = r->getParent();
        if (curr == -1)
            return Transform{};
        else {
            t = curr.get<Transform>();
            r = curr.get<Relationship>();
        }
    } while (true);
}

ATTA_CPU_GPU Transform Transform::operator*(const Transform& o) const {
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

ATTA_CPU_GPU Transform Transform::operator/(const Transform& o) const {
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
