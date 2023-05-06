//--------------------------------------------------
// Atta Component Module
// transform.h
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_TRANSFORM_H
#define ATTA_COMPONENT_COMPONENTS_TRANSFORM_H

#include <atta/component/interface.h>

namespace atta::component {

/// Entity transform
/** Define the entity position, orientation, and
 * scale.
 *
 * Only entities with a Transform can be rendered or
 * interact physically with other entities.
 *
 * The transform data is represented on the entity's local
 * coordinate system, if the entity has a parent
 * (Relationship), the local position/orientation/scale
 * can different from the world position/orientation/scale.
 */
struct Transform final : public Component {
    vec3f position = vec3();     ///< Position
    quat orientation = quat();   ///< Orientation
    vec3f scale = vec3(1, 1, 1); ///< Scale

    /// Get transform in the world coordinate system
    /** If the entity is child of another entity the
     * resulting transform will depend on its parent
     * world transform.
     *
     * The world transform will only differ from the local
     * transform when the entity also has a Relationship.
     */
    ATTA_CPU_GPU Transform getWorldTransform(EntityId entity);

    /// Set position/orientation/scale from the desired world transform
    /** It will convert the world transform to a local space and
     * update position/orientation/scale
     **/
    ATTA_CPU_GPU void setWorldTransform(EntityId entity, Transform worldTransform);

    /// Get transform matrix in the world coordinate system
    ATTA_CPU_GPU mat4 getWorldTransformMatrix(EntityId entity);

    /// Get transform matrix in the local coordinate system
    ATTA_CPU_GPU mat4 getLocalTransformMatrix();

    /// Get transform of any entity in the world coordinate system
    /** Take into account that it is possible that the entity does have a transform component,
     * but someone up in the hierarchy have
     * */
    ATTA_CPU_GPU static Transform getEntityWorldTransform(EntityId entity);

    /// Transform multiplication
    /** Useful to get the world transformation from the parent and local transformations
     * world = parent * local
     **/
    ATTA_CPU_GPU Transform operator*(const Transform& o) const;

    /// Transformation division
    /** Useful to get the local transformation from the parent and world transformations
     * local = world / parent
     **/
    ATTA_CPU_GPU Transform operator/(const Transform& o) const;
};

ATTA_REGISTER_COMPONENT(Transform)
template <>
ComponentDescription& TypedComponentRegistry<Transform>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_TRANSFORM_H
