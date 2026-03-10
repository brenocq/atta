// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/component/components/name.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<Name>::getDescription() {
    static ComponentDescription desc = {"Name", {{AttributeType::VECTOR_CHAR, offsetof(Name, name), "name"}}};

    return desc;
}

void Name::set(std::string n) { strcpy(name, n.c_str()); }

} // namespace atta::component
