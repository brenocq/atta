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

    // Convert world transform to local transform by removing all parent transformations.
    // This transformation needs to be applied from the top-most entity to the current entity

    // Build the stack to store hierarchy from top-most entity to the current entity
    std::stack<EntityId> parentStack;
    Relationship* relationship = component::getComponent<Relationship>(entity);
    while (relationship && relationship->getParent() >= 0) {
        parentStack.push(relationship->getParent());
        relationship = component::getComponent<Relationship>(relationship->getParent());
    }

    // Apply the transformations from the top-most entity to the current entity
    while (!parentStack.empty()) {
        EntityId parent = parentStack.top();
        parentStack.pop();

        // Apply the inverse transformation of the parent
        Transform* ptransform = component::getComponent<Transform>(parent);
        if (ptransform)
            (*this) = (*this) / *ptransform;
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

    // Go up the hierarchy until the first entity with transform component if found
    do {
        // Return the world transform of the first entity with transform component
        if (t)
            return t->getWorldTransform(curr);

        // Return default transform if no transform component is found
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
    world.position = position + orientation * (scale * o.position);
    world.orientation = orientation * o.orientation;
    world.scale = scale * o.scale;

    return world;
}

Transform Transform::operator/(const Transform& o) const {
    // Local = world / parent
    quat oriConj = o.orientation.normalized().inverted();

    Transform local;
    local.position = oriConj * (position - o.position) / o.scale;
    local.orientation = oriConj * orientation;
    local.scale = scale / o.scale;

    return local;
}

} // namespace atta::component
