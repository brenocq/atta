//--------------------------------------------------
// Atta Graphics Module
// interface.h
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_INTERFACE_H
#define ATTA_GRAPHICS_INTERFACE_H

#include <atta/component/base.h>
#include <atta/graphics/apis/graphicsAPI.h>
#include <atta/graphics/viewport.h>
#include <atta/graphics/windows/window.h>

namespace atta::graphics {

void startUp();
void shutDown();
void update();

// Used to create the object (image/pipeline/renderPass/...) based on the current rendererAPI
// e.g.: Manager::create<Pipeline>(pipelineInfo) will create OpenGLPipeline or
// VulkanPipeline or ... depending on the current renderering API
template <typename T, typename... Args>
std::shared_ptr<T> create(Args... args);
std::shared_ptr<GraphicsAPI> getGraphicsAPI();
std::shared_ptr<Window> getWindow();

/** Set which graphics API should be used
 *
 * Can be used to switch between Vulkan and OpenGL. The window and API resources will be recreated.
 */
void setGraphicsAPI(GraphicsAPI::Type type);

//----- Config -----//
float getGraphicsFPS();
void setGraphicsFPS(float graphicsFPS);
float getViewportFPS();
void setViewportFPS(float viewportFPS);
bool getViewportRendering();
void setViewportRendering(bool viewportRendering);

//----- UI -----//
void setUiRenderFunc(std::function<void()> uiRenderFunc);
void setUiShutDownFunc(std::function<void()> uiShutDownFunc);
void setUiStartUpFunc(std::function<void()> uiStartUpFunc);

//----- Viewport -----//
// std::vector<std::shared_ptr<Viewport>> getViewports();
// void clearViewports();
// void addViewport(std::shared_ptr<Viewport> viewport);
// void removeViewport(std::shared_ptr<Viewport> viewport);
// void createDefaultViewports();
// component::EntityId viewportEntityClick(std::shared_ptr<Viewport> viewport, vec2i pos);
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
