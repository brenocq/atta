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
    static ComponentDescription desc = {"Name", {{AttributeType::VECTOR_CHAR, offsetof(Name, name), "name"}}};

    return desc;
}

void Name::set(std::string n) { strcpy(name, n.c_str()); }

} // namespace atta::component
