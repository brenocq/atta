//--------------------------------------------------
// Atta UI Module
// button.cpp
// Date: 2022-10-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/layers/editor/components/button.h>

#include "imgui.h"
#include <atta/graphics/interface.h>

namespace atta::ui {

bool ImageButton(std::string name, float size) {
    return ImGui::ImageButton(graphics::getImGuiImage("icons/" + name + ".png"), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0);
}

} // namespace atta::ui
