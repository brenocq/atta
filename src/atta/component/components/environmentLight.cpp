//--------------------------------------------------
// Atta Component Module
// environmentLightComponent.cpp
// Date: 2022-01-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/environmentLightComponent.h>

namespace atta::component {
template <>
ComponentDescription& TypedComponentRegistry<EnvironmentLightComponent>::getDescription() {
    static ComponentDescription desc = {"Environment Light",
                                        {
                                            {AttributeType::STRINGID, offsetof(EnvironmentLightComponent, sid), "sid", {}, {}, {}, {}},
                                        },
                                        1};

    return desc;
}

EnvironmentLightComponent::EnvironmentLightComponent() {
    if (TypedComponentRegistry<EnvironmentLightComponent>::description->attributeDescriptions[0].options.size())
        sid = std::any_cast<StringId>(*(TypedComponentRegistry<EnvironmentLightComponent>::description->attributeDescriptions[0].options.begin()));
}
} // namespace atta::component
