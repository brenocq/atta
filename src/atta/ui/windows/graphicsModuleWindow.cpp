//--------------------------------------------------
// Atta UI Module
// graphicsModuleWindow.cpp
// Date: 2022-10-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/windows/graphicsModuleWindow.h>

#include <atta/graphics/interface.h>

namespace atta::ui {

GraphicsModuleWindow::GraphicsModuleWindow() {
    initialize("Graphics Module");
    _initialSize = vec2(300.0f, 200.0f);
}

void GraphicsModuleWindow::renderImpl() {
    static std::vector<gfx::GraphicsAPI::Type> supportedGraphicsAPIs{};
    std::vector<const char*> supportedGraphicsAPIsStr{};
    static std::map<gfx::GraphicsAPI::Type, const char*> typeToString = {
        {gfx::GraphicsAPI::Type::OPENGL, "OpenGL"},
        {gfx::GraphicsAPI::Type::VULKAN, "Vulkan"},
    };
    static std::map<const char*, gfx::GraphicsAPI::Type> stringToType = {
        {"OpenGL", gfx::GraphicsAPI::Type::OPENGL},
        {"Vulkan", gfx::GraphicsAPI::Type::VULKAN},
    };

    // Get supported graphics APIs (once)
    if (supportedGraphicsAPIs.empty()) {
        supportedGraphicsAPIs = gfx::getSupportedGraphicsAPIs();
        for (auto api : supportedGraphicsAPIs)
            supportedGraphicsAPIsStr.push_back(typeToString[api]);
    }

    // Get the current API index
    int currentGraphicsAPIIndex = 0;
    gfx::GraphicsAPI::Type currentAPIType = graphics::getGraphicsAPI()->getType();
    for (size_t i = 0; i < supportedGraphicsAPIs.size(); ++i) {
        if (supportedGraphicsAPIs[i] == currentAPIType) {
            currentGraphicsAPIIndex = static_cast<int>(i);
            break;
        }
    }

    // Combo to change the graphics API
    auto getter = [](void* data, int idx) -> const char* {
        std::vector<gfx::GraphicsAPI::Type>* apis = static_cast<std::vector<gfx::GraphicsAPI::Type>*>(data);
        if (idx >= 0 && idx < apis->size()) {
            return typeToString[(*apis)[idx]]; // Assuming typeToString is still available
        }
        return nullptr;
    };
    if (ImGui::Combo("Graphics API", &currentGraphicsAPIIndex, getter, &supportedGraphicsAPIs, supportedGraphicsAPIs.size()))
        gfx::setGraphicsAPI(supportedGraphicsAPIs[currentGraphicsAPIIndex]);

    // Rendering control
    float graphicsFPS = graphics::getGraphicsFPS();
    if (ImGui::DragFloat("Graphics FPS", &graphicsFPS, 0.1f, 10.0f, 60.0f, "%.1f") && graphicsFPS >= 10.0f)
        gfx::setGraphicsFPS(graphicsFPS);
}

} // namespace atta::ui
