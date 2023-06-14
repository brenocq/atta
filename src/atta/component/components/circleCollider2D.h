//--------------------------------------------------
// Atta Component Module
// circleCollider2D.h
// Date: 2022-10-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_CIRCLE_COLLIDER_2D_H
#define ATTA_COMPONENT_COMPONENTS_CIRCLE_COLLIDER_2D_H

#include <atta/component/components/component.h>
#include <atta/component/interface.h>
#include <atta/utils/math/vector.h>

namespace atta::component {

/// %Component to create sphere collider
/** This collider can be used with 2D physics.
 *
 * Transform and RigidBody2D components are necessary for the
 * entity to participate in the physics iteration.
 *
 * The circle will also be scaled by the transform scale. The final
 * radius will be:
 *
 * ```cpp
 * finalRadius = radius * max(transform.worldScale.x, transform.worldScale.y);
 * ```
 */
struct CircleCollider2D final : public Component {
    float radius = 0.5f;        ///< Radius
    vec2 offset = {0.0f, 0.0f}; ///< Offset
};
ATTA_REGISTER_COMPONENT(CircleCollider2D)
template <>
ComponentDescription& TypedComponentRegistry<CircleCollider2D>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_CIRCLE_COLLIDER_2D_H
