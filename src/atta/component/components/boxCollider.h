//--------------------------------------------------
// Atta Component Module
// boxCollider.h
// Date: 2021-11-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_BOX_COLLIDER_H
#define ATTA_COMPONENT_COMPONENTS_BOX_COLLIDER_H

#include <atta/component/interface.h>

namespace atta::component {

/// %Component to create a box collider
/** This collider can be used with 3D physics.
 *
 * Transform and RigidBody components are necessary 
 * for the entity to participate in the physics iteration.
 *
 * The box will also be scaled by the Transform scale.
 */
struct BoxCollider final : public Component {
    vec3 size = {1.0f, 1.0f, 1.0f};   ///< Size
    vec3 offset = {0.0f, 0.0f, 0.0f}; ///< Offset
};
ATTA_REGISTER_COMPONENT(BoxCollider)
template <>
ComponentDescription& TypedRegistry<BoxCollider>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_BOX_COLLIDER_H
