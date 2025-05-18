//--------------------------------------------------
// Atta UI Module
// statusBar.cpp
// Date: 2025-05-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/cmakeConfig.h>
#include <atta/ui/panels/statusBar/statusBar.h>
#include <atta/ui/widgets/align.h>
#include <imgui.h>
#include <imgui_internal.h>

namespace atta::ui {

void StatusBar::render() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 0));

    // Setup panel window class
    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet =
        ImGuiDockNodeFlags_NoDockingSplit | ImGuiDockNodeFlags_NoDockingOverCentralNode | ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoResize;
    ImGui::SetNextWindowClass(&windowClass);

    ImGui::Begin("##AttaPrivateStatusBar", nullptr, ImGuiWindowFlags_NoDecoration);
    {
        std::string versionText = "v" + std::string(ATTA_VERSION);
        ImVec2 versionTextSize = ImGui::CalcTextSize(versionText.c_str());
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - versionTextSize.x);
        ImGui::TextLinkOpenURL(versionText.c_str(), ("https://github.com/brenocq/atta/releases/tag/" + versionText).c_str());
    }
    ImGui::End();

    ImGui::PopStyleVar(1);
}

} // namespace atta::ui
