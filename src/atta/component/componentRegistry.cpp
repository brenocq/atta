// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/component/componentRegistry.h>
#include <atta/component/interface.h>
#include <imgui.h>

namespace atta::component {

void ComponentRegistry::registerToManager() { component::registerComponent(this); }

} // namespace atta::component
