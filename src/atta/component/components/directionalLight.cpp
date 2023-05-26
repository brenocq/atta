//--------------------------------------------------
// Atta Component Module
// directionalLight.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/directionalLight.h>

namespace atta::component {

template <>
ComponentDescription& TypedRegistry<DirectionalLight>::getDescription() {
    static ComponentDescription desc = {"Directional Light",
                                        {{AttributeType::VECTOR_FLOAT32, offsetof(DirectionalLight, intensity), "intensity", 0.0f, 10.0f}}};
    return desc;
}

} // namespace atta::component
