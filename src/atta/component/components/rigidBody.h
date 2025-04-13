//--------------------------------------------------
// Atta Component Module
// rigidBody.h
// Date: 2021-12-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_RIGID_BODY_H
#define ATTA_COMPONENT_COMPONENTS_RIGID_BODY_H

#include <atta/component/interface.h>

namespace atta::component {
/// %Component to make the entity a rigid body.
/** Every entity that has a RigidBody and some collider
 * (like BoxCollider, SphereCollider) component will be added to
 * the physics simulations.
 *
 * Rigid bodies has physical properties like density, friction coefficient,
 * and restitution coefficient. The object mass will be dependent on the
 * collider area and material density.
 */
struct RigidBody final : public Component {
    /// Rigid body type
    /** There are three types of  rigid body:
     *  - **Dynamic**: Will collide with other rigid bodies.
     *  - **Kinematic**: Will not be affected by collisions and move with
     *  constant velocity according to it linearVelocity and angularVelocity
     *  - **Static**: A rigid body with infinite mass, will participate in collisions,
     *  but not be affected by it.
     */
    enum Type : uint32_t { DYNAMIC = 0, KINEMATIC, STATIC };

    Type type = DYNAMIC;
    // Rigid body state
    vec3 linearVelocity = {0.0f, 0.0f, 0.0f};  ///< Linear velocity
    vec3 angularVelocity = {0.0f, 0.0f, 0.0f}; ///< Angular velocity
    // Material properties
    float mass = 1.0f;     ///< Physics material mass
    float friction = 0.5f; ///< Physics material friction coefficient
    /** A restituion of 0 will result in a perfectly inelastic collision. A
     * restitution of 1 will result in a perfectly elastic collision.
     * Any value between 0 and 1 will result in a inelastic collision.
     */
    float restitution = 0.5f; ///< Physics material restitution coefficient
    // Simulation properties
    /** How much the physical body resist to translation
     */
    float linearDamping = 0.0f; ///< Linear damping
    /** How much the physical body resist to rotation
     */
    float angularDamping = 0.0f; ///< Angular damping
    /** A sleeping rigid bodies are not take into account on the physics
     * broad phase, making the physics simulation faster.
     */
    bool allowSleep = true; ///< If the rigid body can sleep
    bool awake = true;      ///< If the rigid body start awake or sleeping

    /// Set linear velocity
    /// @param vel velocity in meters/second
    void setLinearVelocity(vec3 vel);
    /// Set angular velocity
    /// @param omega velocity in radians/second
    void setAngularVelocity(vec3 omega);

    /// Apply a force at a world point. If the force is not applied at the center of mass,
    /// it will generate a torque and affect the angular velocity.
    /// @param force the world force vector in Newtons (N).
    /// @param point the world position of the point of application.
    void applyForce(vec3 force, vec3 point);

    /// Apply a force to the body's center of mass.
    /// @param force the world force vector in Newtons (N).
    void applyForceToCenter(vec3 force);

    /// Apply a torque. This affects the angular velocity without affecting
    /// the linear velocity of the center of mass.
    /// @param torque about the z-axis (out of the screen), usually in N-m.
    void applyTorque(vec3 torque);
};
ATTA_REGISTER_COMPONENT(RigidBody)
template <>
ComponentDescription& TypedComponentRegistry<RigidBody>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_RIGID_BODY_H
