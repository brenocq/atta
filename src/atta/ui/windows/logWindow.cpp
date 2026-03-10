// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/ui/windows/logWindow.h>
#include <imgui.h>
#include <implot.h>

namespace atta::ui {

void LogWindow::render() {
    ImGui::Begin("Log");
    ImGui::End();
}

} // namespace atta::ui
