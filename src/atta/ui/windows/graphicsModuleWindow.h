// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_UI_WINDOWS_GRAPHICS_MODULE_WINDOW_H
#define ATTA_UI_WINDOWS_GRAPHICS_MODULE_WINDOW_H

#include <atta/graphics/interface.h>
#include <atta/ui/windows/window.h>

namespace atta::ui {

class GraphicsModuleWindow : public Window<GraphicsModuleWindow> {
  private:
    GraphicsModuleWindow();
    void renderImpl();

    friend Window<GraphicsModuleWindow>;
};

} // namespace atta::ui

#endif // ATTA_UI_WINDOWS_GRAPHICS_MODULE_WINDOW_H
