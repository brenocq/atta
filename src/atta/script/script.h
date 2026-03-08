// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#include <atta/component/interface.h>

namespace atta::script {

class Script {
  public:
    Script() = default;
    virtual ~Script() {}
    virtual void update(component::Entity entity, float dt) = 0;
};

} // namespace atta::script
