//--------------------------------------------------
// Atta Component Module
// sphereCollider.h
// Date: 2021-11-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_SPHERE_COLLIDER_H
#define ATTA_COMPONENT_COMPONENTS_SPHERE_COLLIDER_H
#include <atta/component/components/component.h>
#include <atta/component/interface.h>
#include <atta/utils/math/vector.h>

namespace atta::component {

/// %Component to create sphere collider
/** This collider can be used both with 3D physics.
 *
 * Transform and RigidBody components are necessary for the
 * entity to participate in the physics iteration.
 *
 * The sphere will also be scaled by the transform scale. The final
 * radius will be:
 *
 * ```cpp
 * finalRadius = radius * max(transform.worldScale.x, transform.worldScale.y, transform.worldScale.z);
 * ```
 */
struct SphereCollider final : public Component {
    float radius = 0.5f;              ///< Radius
    vec3 offset = {0.0f, 0.0f, 0.0f}; ///< Offset

    /// Get sphere radius and position in world coordinates
    void getWorldData(EntityId eid, float& worldRadius, vec3& worldPos);
};
ATTA_REGISTER_COMPONENT(SphereCollider)
template <>
ComponentDescription& TypedRegistry<SphereCollider>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_SPHERE_COLLIDER_H
