// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/component/interface.h>

namespace atta::component {

struct EnvironmentLight final : public Component {
    EnvironmentLight();
    StringId sid; ///< Environment texture relative path
};
ATTA_REGISTER_COMPONENT(EnvironmentLight)
template <>
ComponentDescription& TypedComponentRegistry<EnvironmentLight>::getDescription();

} // namespace atta::component
