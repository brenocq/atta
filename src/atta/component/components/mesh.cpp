//--------------------------------------------------
// Atta Component Module
// meshComponent.cpp
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/meshComponent.h>

namespace atta::component {
template <>
ComponentDescription& TypedComponentRegistry<MeshComponent>::getDescription() {
    static ComponentDescription desc = {"Mesh",
                                        {
                                            {AttributeType::STRINGID, offsetof(MeshComponent, sid), "sid", {}, {}, {}, {}},
                                        }};

    return desc;
}

MeshComponent::MeshComponent() {
    if (TypedComponentRegistry<MeshComponent>::description->attributeDescriptions[0].options.size())
        sid = std::any_cast<StringId>(*(TypedComponentRegistry<MeshComponent>::description->attributeDescriptions[0].options.begin()));
}

} // namespace atta::component
