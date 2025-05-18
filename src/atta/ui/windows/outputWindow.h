//--------------------------------------------------
// Atta UI Module
// outputWindow.h
// Date: 2021-12-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_WINDOWS_OUTPUT_WINDOW_H
#define ATTA_UI_WINDOWS_OUTPUT_WINDOW_H

#include <atta/ui/windows/window.h>

namespace atta::ui {

class OutputWindow : public Window<OutputWindow> {
  private:
    OutputWindow();

    void renderImpl();

    friend Window<OutputWindow>;
};

} // namespace atta::ui

#endif // ATTA_UI_WINDOWS_OUTPUT_WINDOW_H
