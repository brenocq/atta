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
void update() { Manager::getInstance().updateImpl(); }

void pushLayer(Layer* layer) { Manager::getInstance().pushLayerImpl(layer); }

std::shared_ptr<RendererAPI> getRendererAPI() { return Manager::getInstance().getRendererAPIImpl(); };

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
