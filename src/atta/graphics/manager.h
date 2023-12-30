//--------------------------------------------------
// Atta Graphics Module
// manager.h
// Date: 2021-08-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_GRAPHICS_MANAGER_H
#define ATTA_GRAPHICS_GRAPHICS_MANAGER_H

#include <atta/graphics/compute/entityClick.h>
#include <atta/graphics/framebuffer.h>
#include <atta/graphics/image.h>
#include <atta/graphics/indexBuffer.h>
#include <atta/graphics/mesh.h>
#include <atta/graphics/pipeline.h>
#include <atta/graphics/renderPass.h>
#include <atta/graphics/renderQueue.h>
#include <atta/graphics/shader.h>
#include <atta/graphics/vertexBuffer.h>
#include <atta/resource/resources/image.h>

namespace atta::graphics {

class Manager final {
  public:
    Manager();
    static Manager& getInstance();

    friend void startUp();
    friend void shutDown();
    friend void update();
    template <typename T, typename... Args>
    friend std::shared_ptr<T> create(Args... args);

    friend void setGraphicsAPI(GraphicsAPI::Type type);

    friend float getGraphicsFPS();
    friend void setGraphicsFPS(float graphicsFPS);
    friend float getViewportFPS();
    friend void setViewportFPS(float viewportFPS);
    friend bool getViewportRendering();
    friend void setViewportRendering(bool viewportRendering);

    friend void setUiRenderFunc(std::function<void()> uiRenderFunc);
    friend void setUiStartUpFunc(std::function<void()> uiStartUpFunc);
    friend void setUiShutDownFunc(std::function<void()> uiShutDownFunc);

    friend std::shared_ptr<GraphicsAPI> getGraphicsAPI();
    friend std::shared_ptr<Window> getWindow();
    friend std::vector<std::shared_ptr<Viewport>> getViewports();
    friend void clearViewports();
    friend void addViewport(std::shared_ptr<Viewport> viewport);
    friend void removeViewport(std::shared_ptr<Viewport> viewport);
    friend void createDefaultViewports();
    friend component::EntityId viewportEntityClick(std::shared_ptr<Viewport> viewport, vec2i pos);
    friend void* getImGuiImage(StringId sid);

    const std::unordered_map<StringId, std::shared_ptr<Mesh>>& getMeshes() const;
    const std::unordered_map<StringId, std::shared_ptr<Image>>& getImages() const;

  private:
    void startUpImpl();
    void shutDownImpl();
    void updateImpl();
    template <typename T, typename... Args>
    std::shared_ptr<T> createImpl(Args... args);

    template <typename T, typename TOpenGL, typename TVulkan, typename... Args>
    std::shared_ptr<T> createSpecific(Args... args);

    std::shared_ptr<GraphicsAPI> getGraphicsAPIImpl() const;
    std::shared_ptr<Window> getWindowImpl() const;

    void setGraphicsAPIImpl(GraphicsAPI::Type type);
    void recreateGraphicsAPI();

    std::vector<std::shared_ptr<Viewport>>& getViewportsImpl();
    void clearViewportsImpl();
    void addViewportImpl(std::shared_ptr<Viewport> viewport);
    void removeViewportImpl(std::shared_ptr<Viewport> viewport);
    void createDefaultViewportsImpl();
    component::EntityId viewportEntityClickImpl(std::shared_ptr<Viewport> viewport, vec2i pos);
    void* getImGuiImageImpl(StringId sid);

    gfx::Image::Format convertFormat(res::Image::Format format) const;

    // Handle resources
    void syncResources();
    void onMeshLoadEvent(event::Event& event);
    void onMeshUpdateEvent(event::Event& event);
    void onImageLoadEvent(event::Event& event);
    void onImageUpdateEvent(event::Event& event);
    void createMesh(StringId sid);
    void createImage(StringId sid);

    std::shared_ptr<Window> _window;
    float _graphicsFPS; ///< Desired graphics FPS

    // Handle
    GraphicsAPI::Type _desiredGraphicsAPI;
    std::shared_ptr<GraphicsAPI> _graphicsAPI;

    // Resource binding
    std::unordered_map<StringId, std::shared_ptr<Mesh>> _meshes;
    std::unordered_map<StringId, std::shared_ptr<Image>> _images;

    // UI
    std::function<void()> _uiRenderFunc;
    std::function<void()> _uiStartUpFunc;  ///< Used to restart UI when graphics API changes
    std::function<void()> _uiShutDownFunc; ///< Used to restart UI when graphics API changes

    // Viewports
    std::vector<std::shared_ptr<Viewport>> _viewports;
    std::vector<std::shared_ptr<Viewport>> _viewportsNext; ///< Being used for now to update the viewports in the next frame without breaking imgui
    bool _swapViewports;                                   ///< If _viewports should be swapped
    float _viewportFPS;                                    ///< Desired viewport FPS (UI module handles the viewport rendering)
    bool _viewportRendering;                               ///< If should render the viewport

    // Compute
    std::unique_ptr<EntityClick> _computeEntityClick;
};

} // namespace atta::graphics

#include <atta/graphics/manager.inl>

#endif // ATTA_GRAPHICS_GRAPHICS_MANAGER_H
