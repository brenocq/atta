// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/physics/interface.h>
#include <atta/ui/windows/window.h>

namespace atta::ui {

class PhysicsModuleWindow : public Window<PhysicsModuleWindow> {
  private:
    PhysicsModuleWindow();
    void renderImpl();

    friend Window<PhysicsModuleWindow>;
};

} // namespace atta::ui
