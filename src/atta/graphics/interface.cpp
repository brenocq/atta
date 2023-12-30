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

//----- Config -----//
float getGraphicsFPS() { return Manager::getInstance()._graphicsFPS; }
void setGraphicsFPS(float graphicsFPS) { Manager::getInstance()._graphicsFPS = graphicsFPS; }
float getViewportFPS() { return Manager::getInstance()._viewportFPS; }
void setViewportFPS(float viewportFPS) { Manager::getInstance()._viewportFPS = viewportFPS; }
bool getViewportRendering() { return Manager::getInstance()._viewportRendering; }
void setViewportRendering(bool viewportRendering) { Manager::getInstance()._viewportRendering = viewportRendering; }

//----- UI -----//
void setUiRenderFunc(std::function<void()> uiRenderFunc) { Manager::getInstance()._uiRenderFunc = uiRenderFunc; }
void setUiShutDownFunc(std::function<void()> uiShutDownFunc) { Manager::getInstance()._uiShutDownFunc = uiShutDownFunc; }
void setUiStartUpFunc(std::function<void()> uiStartUpFunc) { Manager::getInstance()._uiStartUpFunc = uiStartUpFunc; }

//----- Viewport -----//
std::vector<std::shared_ptr<Viewport>> getViewports() { return Manager::getInstance().getViewportsImpl(); }
void clearViewports() { Manager::getInstance().clearViewportsImpl(); }
void addViewport(std::shared_ptr<Viewport> viewport) { Manager::getInstance().addViewportImpl(viewport); }
void removeViewport(std::shared_ptr<Viewport> viewport) { Manager::getInstance().removeViewportImpl(viewport); }
void createDefaultViewports() { return Manager::getInstance().createDefaultViewportsImpl(); }
component::EntityId viewportEntityClick(std::shared_ptr<Viewport> viewport, vec2i pos) {
    return Manager::getInstance().viewportEntityClickImpl(viewport, pos);
}

void* getImGuiImage(StringId sid) { return Manager::getInstance().getImGuiImageImpl(sid); }

} // namespace atta::graphics
