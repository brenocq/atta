//--------------------------------------------------
// Atta Component System
// boxColliderComponent.h
// Date: 2021-11-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_BOX_COLLIDER_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_BOX_COLLIDER_COMPONENT_H
#include <atta/componentSystem/componentManager.h>

namespace atta
{
	/// %Component to create a box collider
	/** This collider can be used both with 2D and 3D physics.
     * If using 2D physics, will be a square instead of a box.
     *
     * TransformComponent and RigidBody2DComponent are necessary for the 
	 * entity to participate in the physics iteration.
	 *
	 * The box will also be scaled by the TransformComponent world scale. 
     * 
     * When using 2D physics, the best
     * box face is chosen to create the square. If the 2D physics plane
	 * is set to Z, the face aligned with the Z plane will be chosen.
     *
     * The box should be aligned with the world axis when using 2D 
     * physics, you should only do rotations around the physics plane 
     * normal.
	 */
    struct BoxColliderComponent final : public Component
    {
		vec3 size = { 1.0f, 1.0f, 1.0f };///< Size
		vec3 offset = { 0.0f, 0.0f, 0.0f };///< Offset
    };
    ATTA_REGISTER_COMPONENT(BoxColliderComponent)
    template<> ComponentDescription& TypedComponentRegistry<BoxColliderComponent>::getDescription();
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_BOX_COLLIDER_COMPONENT_H
