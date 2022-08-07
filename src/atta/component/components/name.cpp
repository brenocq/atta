//--------------------------------------------------
// Atta Component Module
// nameComponent.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/nameComponent.h>

namespace atta::component {
template <>
ComponentDescription& TypedComponentRegistry<NameComponent>::getDescription() {
    static ComponentDescription desc = {"Name", {{AttributeType::CHAR, offsetof(NameComponent, name), "name"}}};

    return desc;
}
} // namespace atta::component
