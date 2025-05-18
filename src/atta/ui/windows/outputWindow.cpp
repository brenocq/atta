//--------------------------------------------------
// Atta UI Module
// outputWindow.cpp
// Date: 2021-12-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/windows/outputWindow.h>

namespace atta::ui {

OutputWindow::OutputWindow() {
    initialize("Output", DockPosition::DOWN);
    _open = true;
}

void OutputWindow::renderImpl() {
    // TODO Implement this: https://github.com/brenocq/atta/issues/83
}

} // namespace atta::ui
