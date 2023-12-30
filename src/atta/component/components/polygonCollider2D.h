//--------------------------------------------------
// Atta Component Module
// polygonCollider2D.h
// Date: 2022-10-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_POLYGON_COLLIDER_2D_H
#define ATTA_COMPONENT_COMPONENTS_POLYGON_COLLIDER_2D_H

#include <atta/component/interface.h>

namespace atta::component {

/// %Component to create a polygon collider
/** This collider can be used only for 2D physics.
 *
 * Transform and RigidBody2D components are necessary
 * for the entity to participate in the physics iteration.
 *
 * This collider allows to define the collision shape from a number of points
 *
 * The polygon will also be scaled by the Transform.
 */
struct PolygonCollider2D final : public Component {
    vec2 offset = {0.0f, 0.0f}; ///< Offset
    std::vector<vec2> points;   ///< Polygon points
};
ATTA_REGISTER_COMPONENT(PolygonCollider2D)
template <>
ComponentDescription& TypedComponentRegistry<PolygonCollider2D>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_POLYGON_COLLIDER_2D_H
