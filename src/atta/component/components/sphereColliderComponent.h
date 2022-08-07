//--------------------------------------------------
// Atta Component Module
// sphereColliderComponent.h
// Date: 2021-11-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_SPHERE_COLLIDER_COMPONENT_H
#define ATTA_COMPONENT_COMPONENTS_SPHERE_COLLIDER_COMPONENT_H
#include <atta/component/components/component.h>
#include <atta/component/componentManager.h>
#include <atta/core/math/vector.h>

namespace atta
{
	/// %Component to create sphere collider
	/** This collider can be used both with 2D and 3D physics.
     * If using 2D physics, will be a circle instead of a sphere.
	 *
	 * TransformComponent and RigidBody2DComponent are necessary for the 
	 * entity to participate in the physics iteration.
	 *
	 * The sphere will also be scaled by the transform scale. The final
     * radius will be:
	 *
	 * ```cpp
	 * finalRadius = radius*max(transform.worldScale.x, transform.worldScale.y, transform.worldScale.z);
	 * ```
	 */
    struct SphereColliderComponent final : public Component
    {
		float radius = 0.5f;///< Radius
		vec3 offset = { 0.0f, 0.0f, 0.0f };///< Offset

        /// Get sphere radius and position in world coordinates
        void getWorldData(EntityId eid, float& worldRadius, vec3& worldPos);
    };
    ATTA_REGISTER_COMPONENT(SphereColliderComponent)
    template<> ComponentDescription& TypedComponentRegistry<SphereColliderComponent>::getDescription();
}

#endif// ATTA_COMPONENT_COMPONENTS_SPHERE_COLLIDER_COMPONENT_H
