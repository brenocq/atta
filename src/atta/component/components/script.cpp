//--------------------------------------------------
// Atta Component Module
// script.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/script.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<Script>::getDescription() {
    static ComponentDescription desc = {"Script",
                                        {
                                            {AttributeType::STRINGID, offsetof(Script, sid), "sid", {}, {}, {}, {}},
                                        }};

    return desc;
}

} // namespace atta::component
