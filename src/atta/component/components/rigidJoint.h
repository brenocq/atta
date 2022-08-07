//--------------------------------------------------
// Atta Component Module
// rigidJointComponent.h
// Date: 2021-12-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_RIGID_JOINT_COMPONENT_H
#define ATTA_COMPONENT_COMPONENTS_RIGID_JOINT_COMPONENT_H
#include <atta/component/base.h>
#include <atta/component/componentManager.h>

namespace atta::component {
/// Joint to keep entities fixed to each other
/** The relative position and orientation between the entities will not change */
struct RigidJointComponent : public Component {
    EntityId bodyA; ///< First attached body
    EntityId bodyB; ///< Second attached body
};
ATTA_REGISTER_COMPONENT(RigidJointComponent)
template <>
ComponentDescription& TypedComponentRegistry<RigidJointComponent>::getDescription();
} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_RIGID_JOINT_COMPONENT_BASE_H
