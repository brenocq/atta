// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_GRAPHICS_WINDOWS_NULL_WINDOW_H
#define ATTA_GRAPHICS_WINDOWS_NULL_WINDOW_H
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

#endif // ATTA_GRAPHICS_WINDOWS_NULL_WINDOW_H
