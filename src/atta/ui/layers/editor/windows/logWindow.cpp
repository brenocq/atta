//--------------------------------------------------
// Atta UI Module
// logWindow.cpp
// Date: 2021-12-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/layers/editor/windows/logWindow.h>
#include <imgui.h>

namespace atta::ui
{
    void LogWindow::render()
    {
        ImGui::Begin("Log");
        ImGui::Text("Logging not implemented yet");
        ImGui::End();
    }
}
