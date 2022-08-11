//--------------------------------------------------
// Atta Graphics Module
// manager.h
// Date: 2021-08-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_GRAPHICS_MANAGER_H
#define ATTA_GRAPHICS_GRAPHICS_MANAGER_H

#include <atta/graphics/framebuffer.h>
#include <atta/graphics/image.h>
#include <atta/graphics/indexBuffer.h>
#include <atta/graphics/pipeline.h>
#include <atta/graphics/renderPass.h>
#include <atta/graphics/shader.h>
#include <atta/graphics/shaderGroup.h>
#include <atta/graphics/vertexBuffer.h>

#include <atta/graphics/compute/entityClick.h>

namespace atta::graphics {

class Manager final {
  public:
    static Manager& getInstance();

    friend void startUp();
    friend void shutDown();
    friend void update();
    friend void pushLayer(Layer* layer);
    template <typename T, typename... Args>
    friend std::shared_ptr<T> create(Args... args);
    friend std::shared_ptr<RendererAPI> getRendererAPI();
    friend std::vector<std::shared_ptr<Viewport>> getViewports();
    friend void clearViewports();
    friend void addViewport(std::shared_ptr<Viewport> viewport);
    friend void removeViewport(std::shared_ptr<Viewport> viewport);
    friend void createDefaultViewports();
    friend component::EntityId viewportEntityClick(std::shared_ptr<Viewport> viewport, vec2i pos);
    friend void* getImGuiImage(StringId sid);

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
