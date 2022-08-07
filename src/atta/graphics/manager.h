//--------------------------------------------------
// Atta Graphics Module
// manager.h
// Date: 2021-08-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_GRAPHICS_MANAGER_H
#define ATTA_GRAPHICS_GRAPHICS_MANAGER_H

#include <atta/graphics/layers/layerStack.h>
#include <atta/graphics/viewport.h>
#include <atta/graphics/windows/window.h>

#include <atta/graphics/framebuffer.h>
#include <atta/graphics/image.h>
#include <atta/graphics/indexBuffer.h>
#include <atta/graphics/pipeline.h>
#include <atta/graphics/renderPass.h>
#include <atta/graphics/rendererAPI.h>
#include <atta/graphics/shader.h>
#include <atta/graphics/shaderGroup.h>
#include <atta/graphics/vertexBuffer.h>

#include <atta/graphics/compute/entityClick.h>

#include <atta/component/base.h>

namespace atta::graphics {

class Manager final {
  public:
    static Manager& getInstance();
    static void startUp();
    static void shutDown();

    static void update();
    static void pushLayer(Layer* layer);

    // Used to create the object (image/pipeline/renderPass/...) based on the current rendererAPI
    // e.g.: Manager::create<Pipeline>(pipelineInfo) will create OpenGLPipeline or
    // VulkanPipeline or ... depending on the current renderering API
    template <typename T, typename... Args>
    static std::shared_ptr<T> create(Args... args) {
        return getInstance().createImpl<T>(args...);
    }
    static std::shared_ptr<RendererAPI> getRendererAPI() { return getInstance().getRendererAPIImpl(); };

    //----- Viewport -----//
    static std::vector<std::shared_ptr<Viewport>> getViewports() { return getInstance().getViewportsImpl(); }
    static void clearViewports();
    static void addViewport(std::shared_ptr<Viewport> viewport);
    static void removeViewport(std::shared_ptr<Viewport> viewport);
    static void createDefaultViewports();
    static component::EntityId viewportEntityClick(std::shared_ptr<Viewport> viewport, vec2i pos);

    static void* getImGuiImage(StringId sid) { return getInstance().getImGuiImageImpl(sid); }

  private:
    void startUpImpl();
    void shutDownImpl();
    void updateImpl();
    void pushLayerImpl(Layer* layer);
    template <typename T, typename... Args>
    std::shared_ptr<T> createImpl(Args... args);

    template <typename T, typename TOpenGL, typename TVulkan, typename... Args>
    std::shared_ptr<T> createSpecific(Args... args);

    std::shared_ptr<RendererAPI> getRendererAPIImpl() const { return _rendererAPI; };
    std::vector<std::shared_ptr<Viewport>>& getViewportsImpl() { return _viewports; };
    void clearViewportsImpl();
    void addViewportImpl(std::shared_ptr<Viewport> viewport);
    void removeViewportImpl(std::shared_ptr<Viewport> viewport);
    void createDefaultViewportsImpl();
    component::EntityId viewportEntityClickImpl(std::shared_ptr<Viewport> viewport, vec2i pos);

    void* getImGuiImageImpl(StringId sid) const { return _rendererAPI->getImGuiImage(sid); }

    std::shared_ptr<Window> _window;
    std::shared_ptr<RendererAPI> _rendererAPI;

    // Layer stack
    std::unique_ptr<LayerStack> _layerStack;

    // Viewports
    std::vector<std::shared_ptr<Viewport>> _viewports;
    std::vector<std::shared_ptr<Viewport>> _viewportsNext; // Being used for now to update the viewports in the next frame without breaking imgui
    bool _swapViewports;

    // Compute
    std::unique_ptr<EntityClick> _computeEntityClick;
};

} // namespace atta::graphics

#include <atta/graphics/manager.inl>

#endif // ATTA_GRAPHICS_GRAPHICS_MANAGER_H
