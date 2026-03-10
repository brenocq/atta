// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#include <atta/graphics/windows/window.h>

namespace atta::graphics {

class NullWindow final : public Window {
  public:
    NullWindow(const CreateInfo& info);
    virtual ~NullWindow() = default;

    void update() override {}
    void swapBuffers() override {}
};

} // namespace atta::graphics
