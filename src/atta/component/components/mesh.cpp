//--------------------------------------------------
// Atta Component Module
// mesh.cpp
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/mesh.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<Mesh>::getDescription() {
    static ComponentDescription desc = {"Mesh",
                                        {
                                            {AttributeType::STRINGID, offsetof(Mesh, sid), "sid", {}, {}, {}, {}},
                                        }};

    return desc;
}

Mesh::Mesh() {
    if (TypedComponentRegistry<Mesh>::description->attributeDescriptions[0].options.size())
        sid = std::any_cast<StringId>(*(TypedComponentRegistry<Mesh>::description->attributeDescriptions[0].options.begin()));
}

} // namespace atta::component
