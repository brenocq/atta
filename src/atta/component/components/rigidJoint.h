//--------------------------------------------------
// Atta Component Module
// rigidJoint.h
// Date: 2021-12-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_RIGID_JOINT_H
#define ATTA_COMPONENT_COMPONENTS_RIGID_JOINT_H
#include <atta/component/base.h>
#include <atta/component/interface.h>

namespace atta::component {

/// Joint to keep entities fixed to each other
/** The relative position and orientation between the entities will not change */
struct RigidJoint : public Component {
    EntityId bodyA; ///< First attached body
    EntityId bodyB; ///< Second attached body
};
ATTA_REGISTER_COMPONENT(RigidJoint)
template <>
ComponentDescription& TypedRegistry<RigidJoint>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_RIGID_JOINT_COMPONENT_BASE_H
