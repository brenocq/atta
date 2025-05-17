//--------------------------------------------------
// Atta UI Module
// logWindow.cpp
// Date: 2021-12-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/windows/logWindow.h>
#include <imgui.h>
#include <implot.h>

namespace atta::ui {

void LogWindow::render() {
    ImGui::Begin("Log##Atta");
    ImGui::End();
}

} // namespace atta::ui
