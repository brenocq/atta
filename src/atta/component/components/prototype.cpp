//--------------------------------------------------
// Atta Component Module
// prototype.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/prototype.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<Prototype>::getDescription() {
    static ComponentDescription desc = {"Prototype",
                                        {{AttributeType::UINT64, offsetof(Prototype, maxClones), "maxClones", uint64_t(0), uint64_t(1000), 1}}};

    return desc;
}

} // namespace atta::component
