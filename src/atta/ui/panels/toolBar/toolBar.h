// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_UI_PANELS_TOOL_BAR_TOOL_BAR_H
#define ATTA_UI_PANELS_TOOL_BAR_TOOL_BAR_H

#include <atta/event/event.h>

namespace atta::ui {

class ToolBar {
  public:
    ToolBar() = default;

    void render();
};

} // namespace atta::ui

#endif // ATTA_UI_PANELS_TOOL_BAR_TOOL_BAR_H
