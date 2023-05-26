//--------------------------------------------------
// Atta Component Module
// pointLight.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/pointLight.h>

namespace atta::component {

template <>
ComponentDescription& TypedRegistry<PointLight>::getDescription() {
    static ComponentDescription desc = {"Point Light", {{AttributeType::VECTOR_FLOAT32, offsetof(PointLight, intensity), "intensity", 0.0f, 10.0f}}};

    return desc;
}

} // namespace atta::component
