//--------------------------------------------------
// Atta Graphics Module
// interface.h
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_INTERFACE_H
#define ATTA_GRAPHICS_INTERFACE_H

#include <atta/component/base.h>
#include <atta/graphics/layers/layerStack.h>
#include <atta/graphics/apis/graphicsAPI.h>
#include <atta/graphics/viewport.h>
#include <atta/graphics/windows/window.h>

namespace atta::graphics {

void startUp();
void shutDown();
void update();

void pushLayer(Layer* layer);

// Used to create the object (image/pipeline/renderPass/...) based on the current rendererAPI
// e.g.: Manager::create<Pipeline>(pipelineInfo) will create OpenGLPipeline or
// VulkanPipeline or ... depending on the current renderering API
template <typename T, typename... Args>
std::shared_ptr<T> create(Args... args);
std::shared_ptr<GraphicsAPI> getGraphicsAPI();
std::shared_ptr<Window> getWindow();

//----- Config -----//
float getGraphicsFPS();
void setGraphicsFPS(float graphicsFPS);
float getViewportFPS();
void setViewportFPS(float viewportFPS);
bool getViewportRendering();
void setViewportRendering(bool viewportRendering);

//----- Viewport -----//
std::vector<std::shared_ptr<Viewport>> getViewports();
void clearViewports();
void addViewport(std::shared_ptr<Viewport> viewport);
void removeViewport(std::shared_ptr<Viewport> viewport);
void createDefaultViewports();
component::EntityId viewportEntityClick(std::shared_ptr<Viewport> viewport, vec2i pos);
void* getImGuiImage(StringId sid);

} // namespace atta::graphics

#include <atta/graphics/manager.h>

namespace atta::graphics {

template <typename T, typename... Args>
std::shared_ptr<T> create(Args... args) {
    return Manager::getInstance().createImpl<T>(args...);
}

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_INTERFACE_H
