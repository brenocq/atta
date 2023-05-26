//--------------------------------------------------
// Atta Component Module
// cylinderCollider.h
// Date: 2022-10-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_CYLINDER_COLLIDER_H
#define ATTA_COMPONENT_COMPONENTS_CYLINDER_COLLIDER_H

#include <atta/component/components/component.h>
#include <atta/component/interface.h>
#include <atta/utils/math/vector.h>

namespace atta::component {

/// %Component to create cylinder collider
/** This collider can be used for 3D physics.
 *
 * Transform and RigidBody are necessary for the
 * entity to be participate in the physics iteration.
 */
struct CylinderCollider final : public Component {
    float radius = 0.5f;              ///< Radius
    float height = 1.0f;              ///< Height
    vec3 offset = {0.0f, 0.0f, 0.0f}; ///< Offset
};
ATTA_REGISTER_COMPONENT(CylinderCollider)
template <>
ComponentDescription& TypedRegistry<CylinderCollider>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_CYLINDER_COLLIDER_H
