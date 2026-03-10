// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/graphics/interface.h>
#include <atta/ui/widgets/image.h>

#include "imgui.h"

namespace atta::ui {

void image(std::string name, vec2 size) { ImGui::Image((ImTextureID)(intptr_t)gfx::getImGuiImage(name), ImVec2(size.x, size.y)); }

} // namespace atta::ui
