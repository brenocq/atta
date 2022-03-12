//--------------------------------------------------
// Atta Component System
// rigidBody2DComponent.h
// Date: 2021-11-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_RIGID_BODY_2D_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_RIGID_BODY_2D_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/componentSystem/componentManager.h>
#include <atta/core/math/vector.h>

namespace atta
{
	/// %Component to make the entity a rigid body for 2D physics
	/** Every entity that has a RigidBody2DComponent and some 2D collider
	 * component will be added to the 2D physics simulations.
	 * **Obs:** The entity **must** have SphereColliderComponent or 
	 * BoxColliderComponent.
	 *
	 * Rigid bodies has physical properties like density, friction coefficient, 
	 * and restitution coefficient. The object mass will be dependent on the
	 * collider area and material density.
	 */
    struct RigidBody2DComponent final : public Component
    {
		/// Rigid body type
		/** There are three types of 2D rigid body:
		 *  - **Dynamic**: Will collide with other rigid bodies.
		 *  - **Kinematic**: Will not be affected by collisions and move with 
		 *  constant velocity according to it linearVelocity and angularVelocity
		 *  - **Static**: A rigid body with infinite mass, will participate in collisions,
		 *  but not be affected by it.
		 */
		enum Type : uint32_t
		{
			DYNAMIC = 0,
			KINEMATIC,
			STATIC
		};

		Type type = DYNAMIC;
		// Rigid body state
		vec2 linearVelocity = { 0.0f, 0.0f };///< Linear velocity
		float angularVelocity = 0.0f;///< Angular velocity
		// Material properties
		float mass = 1.0f;///< Physics material mass
		float friction = 0.5f;///< Physics material friction coefficient
		/** A restituion of 0 will result in a perfectly inelastic collision. A 
		 * restitution of 1 will result in a perfectly elastic collision.
		 * Any value between 0 and 1 will result in a inelastic collision.
		 */
		float restitution = 0.5f;///< Physics material restitution coefficient
		// Simulation properties
		/** How much the physical body resist to translation
		 */
		float linearDamping = 0.0f;///< Linear damping
		/** How much the physical body resist to rotation
		 */
		float angularDamping = 0.0f;///< Angular damping
		/** A sleeping rigid bodies are not take into account on the physics
		 * broad phase, making the physics simulation faster.
		 */
		bool allowSleep = true;///< If the rigid body can sleep
		bool awake = true;///< If the rigid body start awake or sleeping
        bool fixedRotation = false;///< Disable rigid body rotation

        /// Set the position of the body's origin and rotation.
        /// @param position position of the body's origin
        /// @param angle orientation in radians
        void setTransform(vec2 position, float angle);
        /// Set linear velocity
        /// @param vel velocity in meters/second
        void setLinearVelocity(vec2 vel);
        /// Set angular velocity
        /// @param omega velocity in radians/second
        void setAngularVelocity(float omega);

        /// Apply a force at a world point. If the force is not applied at the center of mass, 
        /// it will generate a torque and affect the angular velocity. This wakes up the body.
        /// @param force the world force vector in Newtons (N).
        /// @param point the world position of the point of application.
        /// @param wake also wake up the body
        void applyForce(vec2 force, vec2 point, bool wake = true);

        /// Apply a force to the body's center of mass.
        /// @param force the world force vector in Newtons (N).
        /// @param wake also wake up the body
        void applyForceToCenter(vec2 force, bool wake = true);

        /// Apply a torque. This affects the angular velocity without affecting 
        /// the linear velocity of the center of mass.
        /// @param torque about the z-axis (out of the screen), usually in N-m.
        /// @param wake also wake up the body
        void applyTorque(float torque, bool wake = true);
    };
    ATTA_REGISTER_COMPONENT(RigidBody2DComponent)
    template<> ComponentDescription& TypedComponentRegistry<RigidBody2DComponent>::getDescription();
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_RIGID_BODY_2D_COMPONENT_H
