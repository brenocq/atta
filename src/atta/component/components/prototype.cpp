// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/component/components/prototype.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<Prototype>::getDescription() {
    static ComponentDescription desc = {"Prototype",
                                        {{AttributeType::UINT64, offsetof(Prototype, maxClones), "maxClones", uint64_t(0), uint64_t(1000), 1}}};

    return desc;
}

} // namespace atta::component
