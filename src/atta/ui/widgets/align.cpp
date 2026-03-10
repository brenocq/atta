// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/ui/widgets/align.h>

#include "imgui.h"

namespace atta::ui {

void verticalAlign(float height, std::function<void()> func) {
    float originalPosY = ImGui::GetCursorPosY();
    float padding = (ImGui::GetWindowContentRegionMax().y - height) * 0.5f;
    ImGui::SetCursorPosY(originalPosY + padding);
    func();
    ImGui::SetCursorPosY(originalPosY);
}

} // namespace atta::ui
