//--------------------------------------------------
// Atta UI Module
// image.cpp
// Date: 2024-01-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/interface.h>
#include <atta/ui/editor/widgets/image.h>

#include "imgui.h"

namespace atta::ui {

void image(std::string name, vec2 size) { ImGui::Image(gfx::getImGuiImage(name), ImVec2(size.x, size.y)); }

} // namespace atta::ui
