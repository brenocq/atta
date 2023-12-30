//--------------------------------------------------
// Atta UI Module
// graphicsModuleWindow.cpp
// Date: 2022-10-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/editor/moduleWindows/graphicsModuleWindow.h>

#include <atta/graphics/interface.h>

namespace atta::ui {

GraphicsModuleWindow::GraphicsModuleWindow() {
    setName("Graphics Module");
    _initialSize = vec2(300.0f, 200.0f);
}

void GraphicsModuleWindow::renderImpl() {
    // Graphics API control
    const char* graphicsAPIs[] = {
        "OpenGL",
#if ATTA_VULKAN_SUPPORT
        "Vulkan"
#endif
    };
    int currentGraphicsAPI = int(graphics::getGraphicsAPI()->getType());
    if (ImGui::Combo("Graphics API", &currentGraphicsAPI, graphicsAPIs, IM_ARRAYSIZE(graphicsAPIs)))
        graphics::setGraphicsAPI(graphics::GraphicsAPI::Type(currentGraphicsAPI));

    // Rendering control
    float graphicsFPS = graphics::getGraphicsFPS();
    float viewportFPS = graphics::getViewportFPS();
    bool viewportRendering = graphics::getViewportRendering();
    if (ImGui::DragFloat("Graphics FPS", &graphicsFPS, 0.1f, 10.0f, 60.0f, "%.1f") && graphicsFPS >= 10.0f)
        graphics::setGraphicsFPS(graphicsFPS);
    if (ImGui::DragFloat("Viewport FPS", &viewportFPS, 0.1f, 0.01f, graphicsFPS, "%.1f") && viewportFPS >= 0.01f)
        graphics::setViewportFPS(viewportFPS);
    if (ImGui::Checkbox("Viewport rendering", &viewportRendering))
        graphics::setViewportRendering(viewportRendering);
}

} // namespace atta::ui
