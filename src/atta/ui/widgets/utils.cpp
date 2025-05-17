//--------------------------------------------------
// Atta UI Module
// utils.cpp
// Date: 2025-05-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/interface.h>
#include <atta/ui/widgets/utils.h>

#include "imgui.h"
#include "imgui_internal.h"

namespace atta::ui {

void openUrl(const std::string& url) {
    ImGuiContext& g = *GImGui;
    if (g.PlatformIO.Platform_OpenInShellFn != NULL)
        g.PlatformIO.Platform_OpenInShellFn(&g, url.c_str());
}

} // namespace atta::ui
