//--------------------------------------------------
// Atta Graphics Module
// manager.cpp
// Date: 2021-08-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/interface.h>
#include <atta/graphics/manager.h>

#include <atta/graphics/apis/openGL/openGL.h>
#include <atta/graphics/apis/vulkan/vulkan.h>
#include <atta/graphics/cameras/orthographicCamera.h>
#include <atta/graphics/cameras/perspectiveCamera.h>
#include <atta/graphics/renderers/fastRenderer.h>
#include <atta/graphics/renderers/pbrRenderer.h>
#include <atta/graphics/renderers/phongRenderer.h>
#include <atta/graphics/windows/glfwWindow.h>
#include <atta/graphics/windows/nullWindow.h>

#include <atta/event/events/imageLoad.h>
#include <atta/event/events/imageUpdate.h>
#include <atta/event/events/meshLoad.h>
#include <atta/event/interface.h>

#include <atta/event/interface.h>
#include <atta/memory/allocators/stackAllocator.h>
#include <atta/memory/interface.h>

#include <atta/resource/interface.h>

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
    // _graphicsAPI = std::static_pointer_cast<GraphicsAPI>(std::make_shared<OpenGLAPI>(_window));
    _graphicsAPI = std::static_pointer_cast<GraphicsAPI>(std::make_shared<VulkanAPI>(_window));
    _graphicsAPI->startUp();

    //----- Compute Shaders -----//
    // _computeEntityClick = std::make_unique<EntityClick>();

    //----- Create layer stack -----//
    _layerStack = std::make_unique<LayerStack>();

    //----- Create viewports -----//
    createDefaultViewportsImpl();

    //----- Resource sync -----//
    event::subscribe<event::MeshLoad>(BIND_EVENT_FUNC(Manager::onMeshLoadEvent));
    event::subscribe<event::ImageLoad>(BIND_EVENT_FUNC(Manager::onImageLoadEvent));
    event::subscribe<event::ImageUpdate>(BIND_EVENT_FUNC(Manager::onImageUpdateEvent));
    syncResources();
}

void Manager::shutDownImpl() {
    _graphicsAPI->waitDevice();

    // Every reference to the framebuffers must be deleted before window deletion
    for (auto& viewport : _viewports)
        viewport.reset();
    _viewports.clear();
    for (auto& viewport : _viewportsNext)
        viewport.reset();
    _viewportsNext.clear();

    _layerStack.reset();
    _computeEntityClick.reset();
    _graphicsAPI->shutDown();
    _graphicsAPI.reset();
    _window.reset();
}

void Manager::updateImpl() {
    // Graphics fps
    static clock_t gfxLastTime = std::clock();
    const clock_t gfxCurrTime = std::clock();
    const float gfxTimeDiff = float(gfxCurrTime - gfxLastTime) / CLOCKS_PER_SEC;

    if (_graphicsFPS > 0 && (gfxTimeDiff > 1 / _graphicsFPS)) {
        gfxLastTime = gfxCurrTime;
        // Update viewports if it was changed
        if (_swapViewports) {
            _viewports = _viewportsNext;
            _swapViewports = false;
        }

        // Update window events
        _window->update();

        // Render layer stack
        _graphicsAPI->beginFrame();
        _layerStack->render();
        _graphicsAPI->endFrame();
    }
}

void Manager::pushLayerImpl(Layer* layer) { _layerStack->push(layer); }

std::shared_ptr<GraphicsAPI> Manager::getGraphicsAPIImpl() const { return _graphicsAPI; }

std::shared_ptr<Window> Manager::getWindowImpl() const { return _window; }

std::vector<std::shared_ptr<Viewport>>& Manager::getViewportsImpl() { return _viewports; }

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
    viewportInfo.renderer = std::make_shared<FastRenderer>();
    viewportInfo.camera = std::make_shared<PerspectiveCamera>(PerspectiveCamera::CreateInfo{});
    viewportInfo.sid = StringId("Main Viewport");
    _viewportsNext.push_back(std::make_shared<Viewport>(viewportInfo));
    _swapViewports = true;
}

component::EntityId Manager::viewportEntityClickImpl(std::shared_ptr<Viewport> viewport, vec2i pos) {
    return _computeEntityClick->click(viewport, pos);
}

void* Manager::getImGuiImageImpl(StringId sid) const { return _graphicsAPI->getImGuiImage(sid); }

gfx::Image::Format Manager::convertFormat(res::Image::Format format) const {
    switch (format) {
        case res::Image::Format::NONE:
            return gfx::Image::Format::NONE;
        case res::Image::Format::RED8:
            return gfx::Image::Format::RED;
        case res::Image::Format::RGB8:
            return gfx::Image::Format::RGB;
        case res::Image::Format::RGBA8:
            return gfx::Image::Format::RGBA;
        case res::Image::Format::RGB16F:
            return gfx::Image::Format::RGB16F;
        default:
            break;
    }
    LOG_WARN("gfx::Manager", "Could not convert res::Image format to gfx::Image format");
    return gfx::Image::Format::NONE;
}

void Manager::syncResources() {
    // Initialize meshes already loaded
    for (auto meshSid : resource::getResources<resource::Mesh>())
        createMesh(meshSid);
    // Initialize textures already loaded
    for (auto imgSid : resource::getResources<resource::Image>())
        createImage(imgSid);
}

void Manager::onMeshLoadEvent(event::Event& event) {
    event::MeshLoad& e = reinterpret_cast<event::MeshLoad&>(event);
    createMesh(e.sid);
}

void Manager::onImageLoadEvent(event::Event& event) {
    event::ImageLoad& e = reinterpret_cast<event::ImageLoad&>(event);
    createImage(e.sid);
}

void Manager::onImageUpdateEvent(event::Event& event) {
    event::ImageUpdate& e = reinterpret_cast<event::ImageUpdate&>(event);
    // TODO update image
}

void Manager::createMesh(StringId sid) {
    LOG_DEBUG("gfx::Manager", "Create mesh [w]$0[]", sid);
    resource::Mesh* mesh = resource::get<resource::Mesh>(sid.getString());

    VertexBuffer::CreateInfo vertexInfo{};
    vertexInfo.data = (uint8_t*)mesh->getVertices().data();
    vertexInfo.size = mesh->getVertices().size() * sizeof(res::Mesh::Vertex);
    vertexInfo.layout = {{"inPosition", VertexBufferElement::Type::VEC3},
                         {"inNormal", VertexBufferElement::Type::VEC3},
                         {"inTexCoord", VertexBufferElement::Type::VEC2}};
    std::shared_ptr<VertexBuffer> vertexBuffer = create<VertexBuffer>(vertexInfo);

    IndexBuffer::CreateInfo indexInfo{};
    indexInfo.data = (uint8_t*)mesh->getIndices().data();
    indexInfo.size = mesh->getIndices().size() * sizeof(res::Mesh::Index);
    std::shared_ptr<IndexBuffer> indexBuffer = create<IndexBuffer>(indexInfo);

    Mesh::CreateInfo info{};
    info.vertexBuffer = vertexBuffer;
    info.indexBuffer = indexBuffer;
    _meshes[sid] = create<Mesh>(info);
}

void Manager::createImage(StringId sid) {
    LOG_DEBUG("gfx::Manager", "Create image [w]$0[]", sid);
    resource::Image* image = resource::get<resource::Image>(sid);
    if (image == nullptr)
        LOG_WARN("gfx::Manager", "Could not initialize gfx::Image from [w]$0[]", sid);

    Image::CreateInfo info{};
    info.width = image->getWidth();
    info.height = image->getHeight();
    info.data = image->getData();
    info.format = convertFormat(image->getFormat());
    info.debugName = sid;
    _images[sid] = create<Image>(info);
}

//---------- Register API specific implementations ----------//
// For each type, will return the OpenGL, Vulkan, ... implementation based on the current active graphicsAPI
// If the derived (e.g. VulkanImage) has the same type as the base (e.g. Image), it means that does not exists
// an implementation of Image for vulkan

template <>
std::shared_ptr<Image> Manager::createImpl<Image>(Image::CreateInfo info) {
    return createSpecific<Image, gl::Image, vk::Image>(info);
}

template <>
std::shared_ptr<Mesh> Manager::createImpl<Mesh>(Mesh::CreateInfo info) {
    return createSpecific<Mesh, gl::Mesh, vk::Mesh>(info);
}

template <>
std::shared_ptr<Framebuffer> Manager::createImpl<Framebuffer>(Framebuffer::CreateInfo info) {
    return createSpecific<Framebuffer, gl::Framebuffer, vk::Framebuffer>(info);
}

template <>
std::shared_ptr<VertexBuffer> Manager::createImpl<VertexBuffer>(VertexBuffer::CreateInfo info) {
    return createSpecific<VertexBuffer, gl::VertexBuffer, vk::VertexBuffer>(info);
}

template <>
std::shared_ptr<IndexBuffer> Manager::createImpl<IndexBuffer>(IndexBuffer::CreateInfo info) {
    return createSpecific<IndexBuffer, gl::IndexBuffer, vk::IndexBuffer>(info);
}

template <>
std::shared_ptr<RenderPass> Manager::createImpl<RenderPass>(RenderPass::CreateInfo info) {
    return createSpecific<RenderPass, gl::RenderPass, vk::RenderPass>(info);
}

template <>
std::shared_ptr<Shader> Manager::createImpl<Shader>(const char* file) {
    return createImpl<Shader>(fs::path(file));
}

template <>
std::shared_ptr<Shader> Manager::createImpl<Shader>(fs::path file) {
    return createSpecific<Shader, gl::Shader, vk::Shader>(file);
}

template <>
std::shared_ptr<Pipeline> Manager::createImpl<Pipeline>(Pipeline::CreateInfo info) {
    return createSpecific<Pipeline, gl::Pipeline, vk::Pipeline>(info);
}

} // namespace atta::graphics
