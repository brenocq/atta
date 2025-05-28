//--------------------------------------------------
// Atta Component Module
// cylinderCollider.cpp
// Date: 2022-10-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/cylinderCollider.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<CylinderCollider>::getDescription() {
    static ComponentDescription desc = {
        "Cylinder Collider",
        {
            {AttributeType::FLOAT32, offsetof(CylinderCollider, radius), "radius", 0.0001f, 2000.0f, 0.01f},
            {AttributeType::FLOAT32, offsetof(CylinderCollider, height), "height", 0.0001f, 2000.0f, 0.01f},
            {AttributeType::FLOAT32, offsetof(CylinderCollider, offset), "offset", -2000.0f, 2000.0f, 0.01f},
        },
    };

    return desc;
}

} // namespace atta::component
