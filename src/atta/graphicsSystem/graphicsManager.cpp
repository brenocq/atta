//--------------------------------------------------
// Atta Graphics System
// graphicsManager.cpp
// Date: 2021-08-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/memorySystem/memoryManager.h>
#include <atta/memorySystem/allocators/stackAllocator.h>
#include <atta/eventSystem/eventManager.h>

#include <atta/graphicsSystem/windows/nullWindow.h>
#include <atta/graphicsSystem/windows/glfwWindow.h>

#include <atta/graphicsSystem/rendererAPIs/openGL/openGL.h>

#include <atta/graphicsSystem/renderers/fastRenderer.h>
#include <atta/graphicsSystem/renderers/pbrRenderer.h>
#include <atta/graphicsSystem/renderers/phongRenderer.h>

#include <atta/graphicsSystem/cameras/orthographicCamera.h>
#include <atta/graphicsSystem/cameras/perspectiveCamera.h>

#include <atta/graphicsSystem/drawer.h>

namespace atta
{
    GraphicsManager& GraphicsManager::getInstance()
    {
        static GraphicsManager instance;
        return instance;
    }

    void GraphicsManager::startUp() { getInstance().startUpImpl(); }
    void GraphicsManager::startUpImpl()
    {
        //----- System Memory -----//
        // Get main memory
        Allocator* mainAllocator = MemoryManager::getAllocator(SSID("MainAllocator"));
        size_t size = 32*1024*1024;// 32MB
        // Alloc memory inside main memory
        uint8_t* graphicsMemory = static_cast<uint8_t*>(mainAllocator->allocBytes(size, sizeof(uint8_t)));
        // Create new allocator with graphics memory
        StackAllocator* graphics = new StackAllocator(graphicsMemory, size);
        MemoryManager::registerAllocator(SSID("GraphicsAllocator"), static_cast<Allocator*>(graphics));

        //----- Window -----//
        Window::CreateInfo windowInfo {};
        _window = std::static_pointer_cast<Window>(std::make_shared<GlfwWindow>(windowInfo));

        //----- Renderer API -----//
        _rendererAPI = std::static_pointer_cast<RendererAPI>(std::make_shared<OpenGLRenderer>(_window));

        //----- Create layer stack -----//
        _layerStack = std::make_unique<LayerStack>();

        //----- Create viewports -----//
        Viewport::CreateInfo viewportInfo;
        viewportInfo.renderer = std::make_shared<PbrRenderer>();
        viewportInfo.camera = std::static_pointer_cast<Camera>(std::make_shared<PerspectiveCamera>(PerspectiveCamera::CreateInfo{}));
        //viewportInfo.camera = std::static_pointer_cast<Camera>(std::make_shared<OrthographicCamera>(OrthographicCamera::CreateInfo{}));
        viewportInfo.sid = StringId("Main Viewport");
        _viewports.emplace_back(std::make_shared<Viewport>(viewportInfo));

        //Drawer::add(Drawer::Line({0,0,0}, {0,0,1}, {0,0,1,1}, {0,0,1,1}));
        //Drawer::add(Drawer::Line({0,0,0}, {1,0,0}, {1,0,0,1}, {1,0,0,1}));
        //Drawer::add(Drawer::Line({0,0,0}, {0,1,0}, {0,1,0,1}, {0,1,0,1}));
        //Drawer::add(Drawer::Point({1,0,0}, {1,0,0,1}));
        //Drawer::add(Drawer::Point({0,1,0}, {0,1,0,1}));
        //Drawer::add(Drawer::Point({0,0,1}, {0,0,1,1}));
    }

    void GraphicsManager::shutDown() { getInstance().shutDownImpl(); }
    void GraphicsManager::shutDownImpl()
    {
        // Frambuffers must be deleted before window deletion
        for(auto& viewport : _viewports)
            viewport.reset();

        _layerStack.reset();
        _rendererAPI.reset();
        _window.reset();
    }

    void GraphicsManager::update() { getInstance().updateImpl(); }
    void GraphicsManager::updateImpl()
    {
        _window->update();

        _rendererAPI->beginFrame();
        {
            for(auto& viewport : _viewports)
                viewport->render();
            _layerStack->render();
        }
        _rendererAPI->endFrame();

        _window->swapBuffers();
    }

    void GraphicsManager::pushLayer(Layer* layer) { getInstance().pushLayerImpl(layer); }
    void GraphicsManager::pushLayerImpl(Layer* layer)
    {
        _layerStack->push(layer);
    }

    //---------- Register API specific implementations ----------//
    // For each type, will return the OpenGL, Vulkan, ... implementation based on the current active rendererAPI
    // If the derived (e.g. VulkanImage) has the same type as the base (e.g. Image), it means that does not exists
    // an implementation of Image for vulkan

    template <>
    std::shared_ptr<Image> GraphicsManager::createImpl<Image>(Image::CreateInfo info)
    {
        return createSpecific<Image, OpenGLImage, Image>(info);
    }

    template <>
    std::shared_ptr<Framebuffer> GraphicsManager::createImpl<Framebuffer>(Framebuffer::CreateInfo info)
    {
        return createSpecific<Framebuffer, OpenGLFramebuffer, Framebuffer>(info);
    }

    template <>
    std::shared_ptr<VertexBuffer> GraphicsManager::createImpl<VertexBuffer>(VertexBuffer::CreateInfo info)
    {
        return createSpecific<VertexBuffer, VertexBuffer, OpenGLVertexBuffer>(info);
    }

    template <>
    std::shared_ptr<IndexBuffer> GraphicsManager::createImpl<IndexBuffer>(IndexBuffer::CreateInfo info)
    {
        return createSpecific<IndexBuffer, IndexBuffer, OpenGLIndexBuffer>(info);
    }

    template <>
    std::shared_ptr<RenderPass> GraphicsManager::createImpl<RenderPass>(RenderPass::CreateInfo info)
    {
        return createSpecific<RenderPass, OpenGLRenderPass, RenderPass>(info);
    }

    template <>
    std::shared_ptr<Shader> GraphicsManager::createImpl<Shader>(Shader::CreateInfo info)
    {
        return createSpecific<Shader, OpenGLShader, Shader>(info);
    }

    template <>
    std::shared_ptr<ShaderGroup> GraphicsManager::createImpl<ShaderGroup>(ShaderGroup::CreateInfo info)
    {
        return createSpecific<ShaderGroup, OpenGLShaderGroup, ShaderGroup>(info);
    }

    template <>
    std::shared_ptr<Pipeline> GraphicsManager::createImpl<Pipeline>(Pipeline::CreateInfo info)
    {
        return createSpecific<Pipeline, OpenGLPipeline, Pipeline>(info);
    }
}
