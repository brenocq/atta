//--------------------------------------------------
// Atta Graphics Module
// manager.cpp
// Date: 2021-08-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/interface.h>
#include <atta/graphics/manager.h>

#include <atta/graphics/cameras/orthographicCamera.h>
#include <atta/graphics/cameras/perspectiveCamera.h>
#include <atta/graphics/drawer.h>
#include <atta/graphics/rendererAPIs/openGL/openGL.h>
#include <atta/graphics/rendererAPIs/openGL/openGLRenderer.h>
#include <atta/graphics/renderers/fastRenderer.h>
#include <atta/graphics/renderers/pbrRenderer.h>
#include <atta/graphics/renderers/phongRenderer.h>
#include <atta/graphics/windows/glfwWindow.h>
#include <atta/graphics/windows/nullWindow.h>

#include <atta/event/interface.h>
#include <atta/memory/allocators/stackAllocator.h>
#include <atta/memory/interface.h>

namespace atta::graphics {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::startUpImpl() {
    //----- Module Memory -----//
    // Get main memory
    memory::Allocator* mainAllocator = memory::getAllocator(SSID("MainAllocator"));
    size_t size = 32 * 1024 * 1024; // 32MB
    // Alloc memory inside main memory
    uint8_t* graphicsMemory = static_cast<uint8_t*>(mainAllocator->allocBytes(size, sizeof(uint8_t)));
    // Create new allocator with graphics memory
    memory::StackAllocator* graphics = new memory::StackAllocator(graphicsMemory, size);
    memory::registerAllocator(SSID("GraphicsAllocator"), static_cast<memory::Allocator*>(graphics));

    //----- Config -----//
    _graphicsFPS = 30.0f;
    _viewportFPS = 30.0f;
    _viewportRendering = true;

    //----- Window -----//
    Window::CreateInfo windowInfo{};
    _window = std::static_pointer_cast<Window>(std::make_shared<GlfwWindow>(windowInfo));

    //----- Renderer API -----//
    _rendererAPI = std::static_pointer_cast<RendererAPI>(std::make_shared<OpenGLRenderer>(_window));
    _computeEntityClick = std::make_unique<EntityClick>();

    //----- Create layer stack -----//
    _layerStack = std::make_unique<LayerStack>();

    //----- Create viewports -----//
    createDefaultViewportsImpl();
}

void Manager::shutDownImpl() {
    // Every reference to the framebuffers must be deleted before window deletion
    for (auto& viewport : _viewports)
        viewport.reset();
    _viewports.clear();
    for (auto& viewport : _viewportsNext)
        viewport.reset();
    _viewportsNext.clear();

    _layerStack.reset();
    _computeEntityClick.reset();
    _rendererAPI.reset();
    _window.reset();
}

void Manager::updateImpl() {
    // Graphics fps
    static clock_t gfxLastTime = std::clock();
    const clock_t gfxCurrTime = std::clock();
    const float gfxTimeDiff = float(gfxCurrTime - gfxLastTime) / CLOCKS_PER_SEC;

    // Viewports fps
    static clock_t vpLastTime = std::clock();
    const clock_t vpCurrTime = std::clock();
    const float vpTimeDiff = float(vpCurrTime - vpLastTime) / CLOCKS_PER_SEC;

    if (_graphicsFPS > 0 && (gfxTimeDiff > 1 / _graphicsFPS)) {
        gfxLastTime = gfxCurrTime;
        // Update viewports if it was changed
        if (_swapViewports) {
            _viewports = _viewportsNext;
            _swapViewports = false;
        }

        // Update window events
        _window->update();

        // Render viewports
        _rendererAPI->beginFrame();
        {
            if (_viewportRendering && (_viewportFPS > 0 && (vpTimeDiff > 1 / _viewportFPS))) {
                vpLastTime = vpCurrTime;
                for (auto& viewport : _viewports)
                    viewport->render();
            }
            _layerStack->render();
        }
        _rendererAPI->endFrame();

        _window->swapBuffers();
    }
}

void Manager::pushLayerImpl(Layer* layer) { _layerStack->push(layer); }

void Manager::clearViewportsImpl() {
    _viewportsNext.clear();
    _swapViewports = true;
}

void Manager::addViewportImpl(std::shared_ptr<Viewport> viewport) {
    _viewportsNext.push_back(viewport);
    _swapViewports = true;
}

void Manager::removeViewportImpl(std::shared_ptr<Viewport> viewport) {
    // TODO make it work with zero viewports
    if (_viewportsNext.size() > 1) {
        for (unsigned i = 0; i < _viewportsNext.size(); i++)
            if (_viewportsNext[i] == viewport) {
                _viewportsNext.erase(_viewportsNext.begin() + i);
                break;
            }
        _swapViewports = true;
    } else {
        LOG_WARN("graphics::Manager", "It is not possible to have 0 viewports yet");
    }
}

void Manager::createDefaultViewportsImpl() {
    _viewportsNext.clear();

    Viewport::CreateInfo viewportInfo;
    viewportInfo.renderer = std::make_shared<PhongRenderer>();
    viewportInfo.camera = std::make_shared<PerspectiveCamera>(PerspectiveCamera::CreateInfo{});
    viewportInfo.sid = StringId("Main Viewport");
    _viewportsNext.push_back(std::make_shared<Viewport>(viewportInfo));
    _swapViewports = true;
}

component::EntityId Manager::viewportEntityClickImpl(std::shared_ptr<Viewport> viewport, vec2i pos) {
    return _computeEntityClick->click(viewport, pos);
}

//---------- Register API specific implementations ----------//
// For each type, will return the OpenGL, Vulkan, ... implementation based on the current active rendererAPI
// If the derived (e.g. VulkanImage) has the same type as the base (e.g. Image), it means that does not exists
// an implementation of Image for vulkan

template <>
std::shared_ptr<Image> Manager::createImpl<Image>(Image::CreateInfo info) {
    return createSpecific<Image, OpenGLImage, Image>(info);
}

template <>
std::shared_ptr<Framebuffer> Manager::createImpl<Framebuffer>(Framebuffer::CreateInfo info) {
    return createSpecific<Framebuffer, OpenGLFramebuffer, Framebuffer>(info);
}

template <>
std::shared_ptr<VertexBuffer> Manager::createImpl<VertexBuffer>(VertexBuffer::CreateInfo info) {
    return createSpecific<VertexBuffer, VertexBuffer, OpenGLVertexBuffer>(info);
}

template <>
std::shared_ptr<IndexBuffer> Manager::createImpl<IndexBuffer>(IndexBuffer::CreateInfo info) {
    return createSpecific<IndexBuffer, IndexBuffer, OpenGLIndexBuffer>(info);
}

template <>
std::shared_ptr<RenderPass> Manager::createImpl<RenderPass>(RenderPass::CreateInfo info) {
    return createSpecific<RenderPass, OpenGLRenderPass, RenderPass>(info);
}

template <>
std::shared_ptr<Shader> Manager::createImpl<Shader>(Shader::CreateInfo info) {
    return createSpecific<Shader, OpenGLShader, Shader>(info);
}

template <>
std::shared_ptr<ShaderGroup> Manager::createImpl<ShaderGroup>(ShaderGroup::CreateInfo info) {
    return createSpecific<ShaderGroup, OpenGLShaderGroup, ShaderGroup>(info);
}

template <>
std::shared_ptr<Pipeline> Manager::createImpl<Pipeline>(Pipeline::CreateInfo info) {
    return createSpecific<Pipeline, OpenGLPipeline, Pipeline>(info);
}

} // namespace atta::graphics
