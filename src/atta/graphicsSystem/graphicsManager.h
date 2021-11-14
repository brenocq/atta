//--------------------------------------------------
// Atta Graphics System
// graphicsManager.h
// Date: 2021-08-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_GRAPHICS_MANAGER_H
#define ATTA_GRAPHICS_SYSTEM_GRAPHICS_MANAGER_H
#include <atta/graphicsSystem/windows/window.h>
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLRenderer.h>
#include <atta/graphicsSystem/layers/layerStack.h>
#include <atta/graphicsSystem/viewport.h>

#include <atta/graphicsSystem/image.h>
#include <atta/graphicsSystem/framebuffer.h>
#include <atta/graphicsSystem/indexBuffer.h>
#include <atta/graphicsSystem/vertexBuffer.h>
#include <atta/graphicsSystem/renderPass.h>
#include <atta/graphicsSystem/shader.h>
#include <atta/graphicsSystem/shaderGroup.h>
#include <atta/graphicsSystem/pipeline.h>

namespace atta
{
    class GraphicsManager final
    {
    public:
        static GraphicsManager& getInstance();
        static void startUp();
        static void shutDown();

        static void update();
        static void pushLayer(Layer* layer);

        // Used to create the object (image/pipeline/renderPass/...) based on the current rendererAPI
        // e.g.: GraphicsManager::create<Pipeline>(pipelineInfo) will create OpenGLPipeline or 
        // VulkanPipeline or ... depending on the current renderering API
        template <typename T, typename... Args>
        static std::shared_ptr<T> create(Args... args) { return getInstance().createImpl<T>(args...); }

        static std::shared_ptr<RendererAPI> getRendererAPI() { return getInstance().getRendererAPIImpl(); };
        static std::vector<std::shared_ptr<Viewport>> getViewports() { return getInstance().getViewportsImpl(); }
        static void clearViewports();
        static void addViewport(std::shared_ptr<Viewport> viewport);
        static void removeViewport(std::shared_ptr<Viewport> viewport);
        static void createDefaultViewports();

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

        void* getImGuiImageImpl(StringId sid) const { return _rendererAPI->getImGuiImage(sid); }

        std::shared_ptr<Window> _window;
        std::shared_ptr<RendererAPI> _rendererAPI;

        // Layer stack
        std::unique_ptr<LayerStack> _layerStack;

        // Viewports
        std::vector<std::shared_ptr<Viewport>> _viewports;
        std::vector<std::shared_ptr<Viewport>> _viewportsNext;// Being used for now to update the viewports in the next frame without breaking imgui
        bool _swapViewports;
    };
}

#include <atta/graphicsSystem/graphicsManager.inl>
#endif// ATTA_GRAPHICS_SYSTEM_GRAPHICS_MANAGER_H
