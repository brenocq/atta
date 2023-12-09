//--------------------------------------------------
// Atta UI Module
// toolBar.h
// Date: 2021-12-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_EDITOR_TOOL_BAR_TOOL_BAR_H
#define ATTA_UI_EDITOR_TOOL_BAR_TOOL_BAR_H

#include <atta/event/event.h>

namespace atta::ui {

class ToolBar {
  public:
    ToolBar() = default;

    void render();
};

} // namespace atta::ui

#endif // ATTA_UI_EDITOR_TOOL_BAR_TOOL_BAR_H
