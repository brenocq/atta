// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_COMPONENT_COMPONENTS_NAME_H
#define ATTA_COMPONENT_COMPONENTS_NAME_H

#include <atta/component/interface.h>

namespace atta::component {

/// %Component to give an entity a name
struct Name final : public Component {
    char name[32];
    void set(std::string n);
};
ATTA_REGISTER_COMPONENT(Name)
template <>
ComponentDescription& TypedComponentRegistry<Name>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_NAME_H
