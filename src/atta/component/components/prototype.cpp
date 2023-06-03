//--------------------------------------------------
// Atta Component Module
// prototype.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/prototype.h>

namespace atta::component {

#ifndef __NVCC__
template <>
ComponentDescription& TypedRegistry<Prototype>::getDescription() {
    static ComponentDescription desc = {"Prototype",
                                        {{AttributeType::UINT64, offsetof(Prototype, maxClones), "maxClones", uint64_t(0), uint64_t(1000), 1}}};

    return desc;
}
#endif

} // namespace atta::component
