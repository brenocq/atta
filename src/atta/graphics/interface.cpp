//--------------------------------------------------
// Atta Graphics Module
// interface.cpp
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/interface.h>

namespace atta::graphics {

void startUp() { Manager::getInstance().startUpImpl(); }
void shutDown() { Manager::getInstance().shutDownImpl(); }
void update() {
    PROFILE();
    Manager::getInstance().updateImpl();
}

std::shared_ptr<GraphicsAPI> getGraphicsAPI() { return Manager::getInstance().getGraphicsAPIImpl(); };
std::shared_ptr<Window> getWindow() { return Manager::getInstance().getWindowImpl(); };

void setGraphicsAPI(GraphicsAPI::Type type) { Manager::getInstance().setGraphicsAPIImpl(type); }
std::vector<GraphicsAPI::Type> getSupportedGraphicsAPIs() { return Manager::getInstance().getSupportedGraphicsAPIsImpl(); }

//----- Config -----//
float getGraphicsFPS() { return Manager::getInstance()._graphicsFPS; }
void setGraphicsFPS(float graphicsFPS) { Manager::getInstance()._graphicsFPS = graphicsFPS; }

//----- UI -----//
void setUiRenderViewportsFunc(std::function<void()> uiRenderViewportsFunc) { Manager::getInstance()._uiRenderViewportsFunc = uiRenderViewportsFunc; }
void setUiRenderFunc(std::function<void()> uiRenderFunc) { Manager::getInstance()._uiRenderFunc = uiRenderFunc; }
void setUiShutDownFunc(std::function<void()> uiShutDownFunc) { Manager::getInstance()._uiShutDownFunc = uiShutDownFunc; }
void setUiStartUpFunc(std::function<void()> uiStartUpFunc) { Manager::getInstance()._uiStartUpFunc = uiStartUpFunc; }
void* getImGuiImage(StringId sid) { return Manager::getInstance().getImGuiImageImpl(sid); }

} // namespace atta::graphics
