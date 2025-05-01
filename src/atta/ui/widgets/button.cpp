//--------------------------------------------------
// Atta UI Module
// button.cpp
// Date: 2022-10-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/widgets/button.h>

#include "imgui.h"
#include <atta/graphics/interface.h>

namespace atta::ui {

bool imageButton(std::string name, float size) {
    return ImGui::ImageButton(name.c_str(), (ImTextureID)(intptr_t)gfx::getImGuiImage(name), ImVec2(size, size));
}

} // namespace atta::ui
