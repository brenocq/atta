//--------------------------------------------------
// Atta Component Module
// directionalLightComponent.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/directionalLightComponent.h>

namespace atta::component {
template <>
ComponentDescription& TypedComponentRegistry<DirectionalLightComponent>::getDescription() {
    static ComponentDescription desc = {"Directional Light",
                                        {{AttributeType::VECTOR_FLOAT32, offsetof(DirectionalLightComponent, intensity), "intensity", 0.0f, 10.0f}}};
    return desc;
}
} // namespace atta::component
