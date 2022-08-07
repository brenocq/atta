//--------------------------------------------------
// Atta Component Module
// rigidBodyComponent.h
// Date: 2021-12-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_RIGID_BODY_COMPONENT_H
#define ATTA_COMPONENT_COMPONENTS_RIGID_BODY_COMPONENT_H
#include <atta/component/componentManager.h>

namespace atta::component {
/// %Component to make the entity a rigid body.
/** Every entity that has a RigidBodyComponent and some collider
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

    enum Constraint {
        FREEZE_ORIENTATION_X = (1 << 0),
        FREEZE_ORIENTATION_Y = (1 << 1),
        FREEZE_ORIENTATION_Z = (1 << 2),
        FREEZE_POSITION_X = (1 << 3),
        FREEZE_POSITION_Y = (1 << 4),
        FREEZE_POSITION_Z = (1 << 5),
    };

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
                            /** Use RigidBody::Constraint to specify the contraints */
    uint8_t constraints;    ///< Rigid body constraints
};
ATTA_REGISTER_COMPONENT(RigidBodyComponent)
template <>
ComponentDescription& TypedComponentRegistry<RigidBodyComponent>::getDescription();
} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_RIGID_BODY_COMPONENT_H
