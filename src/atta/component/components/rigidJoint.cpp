//--------------------------------------------------
// Atta Component Module
// rigidJoint.cpp
// Date: 2021-12-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/rigidJoint.h>

namespace atta::component {

template <>
ComponentDescription& TypedRegistry<RigidJoint>::getDescription() {
    static ComponentDescription desc = {"Rigid Joint",
                                        {
                                            {AttributeType::INT32, offsetof(RigidJoint, bodyA), "bodyA"},
                                            {AttributeType::INT32, offsetof(RigidJoint, bodyB), "bodyB"},
                                        }};

    return desc;
}

} // namespace atta::component
