//--------------------------------------------------
// Atta Graphics Module
// manager.h
// Date: 2021-08-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_GRAPHICS_MANAGER_H
#define ATTA_GRAPHICS_GRAPHICS_MANAGER_H

#include <atta/graphics/apis/graphicsAPI.h>
#include <atta/graphics/compute/equiToCubemap.h>
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
    friend std::vector<GraphicsAPI::Type> getSupportedGraphicsAPIs();

    friend void setUiRenderViewportsFunc(std::function<void()> uiRenderViewportsFunc);
    friend void setUiRenderFunc(std::function<void()> uiRenderFunc);
    friend void setUiStartUpFunc(std::function<void()> uiStartUpFunc);
    friend void setUiShutDownFunc(std::function<void()> uiShutDownFunc);

    friend std::shared_ptr<GraphicsAPI> getGraphicsAPI();
    friend std::shared_ptr<Window> getWindow();
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
    std::vector<GraphicsAPI::Type> getSupportedGraphicsAPIsImpl() const;
    void recreateGraphicsAPI();

    void* getImGuiImageImpl(StringId sid);

    gfx::Image::Format convertFormat(res::Image::Format format) const;

    // Handle resources
    void createDefaultImages();
    void createDefaultMeshes();
    void syncResources();
    void onMeshLoadEvent(event::Event& event);
    void onMeshUpdateEvent(event::Event& event);
    void onMeshDestroyEvent(event::Event& event);
    void onImageLoadEvent(event::Event& event);
    void onImageUpdateEvent(event::Event& event);
    void createMesh(StringId sid);
    void createImage(StringId sid);

    std::shared_ptr<Window> _window;
    float _graphicsFPS; ///< Desired graphics FPS

    // Handle
    GraphicsAPI::Type _desiredGraphicsAPI;
    std::shared_ptr<GraphicsAPI> _graphicsAPI;

    // Compute shaders
    std::unique_ptr<EquiToCubemap> _equiToCubemap; ///< Used to convert equirectangular res::Image to cubemap gfx::Image

    // Resource binding
    std::unordered_map<StringId, std::shared_ptr<Mesh>> _meshes;
    std::unordered_map<StringId, std::shared_ptr<Image>> _images;

    // UI
    std::function<void()> _uiRenderViewportsFunc;
    std::function<void()> _uiRenderFunc;
    std::function<void()> _uiStartUpFunc;  ///< Used to restart UI when graphics API changes
    std::function<void()> _uiShutDownFunc; ///< Used to restart UI when graphics API changes
};

} // namespace atta::graphics

#include <atta/graphics/manager.inl>

#endif // ATTA_GRAPHICS_GRAPHICS_MANAGER_H
