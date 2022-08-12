//--------------------------------------------------
// Atta Component Module
// name.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/name.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<Name>::getDescription() {
    static ComponentDescription desc = {"Name", {{AttributeType::CHAR, offsetof(Name, name), "name"}}};

    return desc;
}

} // namespace atta::component
