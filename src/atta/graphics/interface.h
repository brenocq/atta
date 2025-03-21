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

/** Get which graphics APIs are supported
 *
 * Returns a vector with the supported graphics APIs.
 */
std::vector<GraphicsAPI::Type> getSupportedGraphicsAPIs();

//----- Config -----//
float getGraphicsFPS();
void setGraphicsFPS(float graphicsFPS);

//----- UI -----//
void setUiRenderViewportsFunc(std::function<void()> uiRenderViewportsFunc);
void setUiRenderFunc(std::function<void()> uiRenderFunc);
void setUiShutDownFunc(std::function<void()> uiShutDownFunc);
void setUiStartUpFunc(std::function<void()> uiStartUpFunc);
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
