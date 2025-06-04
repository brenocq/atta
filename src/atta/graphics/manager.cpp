//--------------------------------------------------
// Atta Graphics Module
// manager.cpp
// Date: 2021-08-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/interface.h>
#include <atta/graphics/manager.h>

#include <atta/graphics/apis/openGL/openGL.h>
#if ATTA_VULKAN_SUPPORT
#include <atta/graphics/apis/vulkan/vulkan.h>
#endif
#include <atta/graphics/cameras/orthographicCamera.h>
#include <atta/graphics/cameras/perspectiveCamera.h>
#include <atta/graphics/renderers/fastRenderer.h>
#include <atta/graphics/renderers/pbrRenderer.h>
#include <atta/graphics/renderers/phongRenderer.h>
#include <atta/graphics/windows/glfwWindow.h>
#include <atta/graphics/windows/nullWindow.h>

#include <atta/event/events/imageLoad.h>
#include <atta/event/events/imageUpdate.h>
#include <atta/event/events/meshDestroy.h>
#include <atta/event/events/meshLoad.h>
#include <atta/event/events/meshUpdate.h>
#include <atta/event/interface.h>

#include <atta/event/interface.h>
#include <atta/memory/allocators/stackAllocator.h>
#include <atta/memory/interface.h>

#include <atta/resource/interface.h>

namespace atta::graphics {

Manager::Manager() {
    setGraphicsAPIImpl(GraphicsAPI::OPENGL);
#if ATTA_VULKAN_SUPPORT
    setGraphicsAPIImpl(GraphicsAPI::VULKAN);
#endif
}

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

    //----- Window -----//
    Window::CreateInfo windowInfo{};
    windowInfo.useOpenGL = _desiredGraphicsAPI == GraphicsAPI::OPENGL;
    _window = std::static_pointer_cast<Window>(std::make_shared<GlfwWindow>(windowInfo));

    //----- Renderer API -----//
#if ATTA_VULKAN_SUPPORT
    if (_desiredGraphicsAPI == GraphicsAPI::VULKAN)
        _graphicsAPI = std::static_pointer_cast<GraphicsAPI>(std::make_shared<VulkanAPI>(_window));
    else
#endif
        _graphicsAPI = std::static_pointer_cast<GraphicsAPI>(std::make_shared<OpenGLAPI>(_window));
    _graphicsAPI->startUp();

    //----- Resource sync -----//
    event::subscribe<event::MeshLoad>(BIND_EVENT_FUNC(Manager::onMeshLoadEvent));
    event::subscribe<event::MeshUpdate>(BIND_EVENT_FUNC(Manager::onMeshUpdateEvent));
    event::subscribe<event::MeshDestroy>(BIND_EVENT_FUNC(Manager::onMeshDestroyEvent));
    event::subscribe<event::ImageLoad>(BIND_EVENT_FUNC(Manager::onImageLoadEvent));
    event::subscribe<event::ImageUpdate>(BIND_EVENT_FUNC(Manager::onImageUpdateEvent));
    syncResources();
}

void Manager::shutDownImpl() {
    event::unsubscribe<event::MeshLoad>(BIND_EVENT_FUNC(Manager::onMeshLoadEvent));
    event::unsubscribe<event::MeshUpdate>(BIND_EVENT_FUNC(Manager::onMeshUpdateEvent));
    event::unsubscribe<event::MeshDestroy>(BIND_EVENT_FUNC(Manager::onMeshDestroyEvent));
    event::unsubscribe<event::ImageLoad>(BIND_EVENT_FUNC(Manager::onImageLoadEvent));
    event::unsubscribe<event::ImageUpdate>(BIND_EVENT_FUNC(Manager::onImageUpdateEvent));

    _graphicsAPI->waitDevice();

    _meshes.clear();
    _images.clear();
    _graphicsAPI->shutDown();
    _graphicsAPI.reset();
    _window.reset();
}

void Manager::updateImpl() {
    if (_graphicsAPI->getType() != _desiredGraphicsAPI)
        recreateGraphicsAPI();

    // Graphics fps
    static clock_t gfxLastTime = std::clock();
    const clock_t gfxCurrTime = std::clock();
    const float gfxTimeDiff = float(gfxCurrTime - gfxLastTime) / CLOCKS_PER_SEC;

    if (_graphicsFPS > 0 && (gfxTimeDiff > 1 / _graphicsFPS)) {
        gfxLastTime = gfxCurrTime;
        // Update window events
        _window->update();

        // Render UI viewports
        if (_uiRenderViewportsFunc)
            _uiRenderViewportsFunc();

        // Render UI
        _graphicsAPI->beginFrame();
        if (_uiRenderFunc)
            _uiRenderFunc();
        _graphicsAPI->endFrame();
    }
}

std::shared_ptr<GraphicsAPI> Manager::getGraphicsAPIImpl() const { return _graphicsAPI; }

std::shared_ptr<Window> Manager::getWindowImpl() const { return _window; }

void Manager::setGraphicsAPIImpl(GraphicsAPI::Type type) {
#if ATTA_VULKAN_SUPPORT
    if (type == GraphicsAPI::VULKAN && !VulkanAPI::isSupported()) {
        LOG_WARN("gfx::Manager", "Failed to set graphics API to Vulkan");
        return;
    }
#endif
    _desiredGraphicsAPI = type;
}

std::vector<GraphicsAPI::Type> Manager::getSupportedGraphicsAPIsImpl() const {
    std::vector<GraphicsAPI::Type> supported;
    supported.push_back(GraphicsAPI::OPENGL);
#if ATTA_VULKAN_SUPPORT
    if (VulkanAPI::isSupported())
        supported.push_back(GraphicsAPI::VULKAN);
#endif
    return supported;
}

void Manager::recreateGraphicsAPI() {
    if (_uiShutDownFunc)
        _uiShutDownFunc();
    shutDownImpl();
    startUpImpl();
    if (_uiStartUpFunc)
        _uiStartUpFunc();
}

void* Manager::getImGuiImageImpl(StringId sid) { return _images[sid]->getImGuiImage(); }

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
        case res::Image::Format::RGB32F:
            return gfx::Image::Format::RGB32F;
        default:
            break;
    }
    LOG_WARN("gfx::Manager", "Could not convert res::Image format to gfx::Image format");
    return gfx::Image::Format::NONE;
}

void Manager::syncResources() {
    _meshes.clear();
    _images.clear();
    // Initialize meshes already loaded
    for (auto meshSid : resource::getResources<resource::Mesh>())
        createMesh(meshSid);
    // Initialize textures already loaded
    for (auto imgSid : resource::getResources<resource::Image>())
        createImage(imgSid);

    //----- Create basic meshes -----//
    // Quad
    {
        static float vertices[] = {
            // Position   UV
            -1.0f, 1.0f,  0.0f, 1.0f, // Top-left
            -1.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
            1.0f,  -1.0f, 1.0f, 0.0f, // Bottom-right
            1.0f,  1.0f,  1.0f, 1.0f, // Top-right
        };
        static uint32_t indices[] = {0, 1, 2, 0, 2, 3};
        VertexBuffer::CreateInfo vertexInfo{};
        vertexInfo.data = (uint8_t*)vertices;
        vertexInfo.size = sizeof(vertices);
        vertexInfo.layout = {};
        vertexInfo.layout.push(BufferLayout::Element::Type::VEC2, "iVertex");
        vertexInfo.layout.push(BufferLayout::Element::Type::VEC2, "iUV");

        IndexBuffer::CreateInfo indexInfo{};
        indexInfo.data = (uint8_t*)indices;
        indexInfo.size = sizeof(indices);

        Mesh::CreateInfo info{};
        info.vertexBufferInfo = vertexInfo;
        info.indexBufferInfo = indexInfo;
        _meshes["atta::gfx::quad"] = create<Mesh>(info);
    }
    // Quad3
    {
        static float vertices[] = {
            // Position         Normal            UV
            -1.0f, 1.0f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // Top-left
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom-left
            1.0f,  -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // Bottom-right
            1.0f,  1.0f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Top-right
        };
        static uint32_t indices[] = {0, 1, 2, 0, 2, 3};
        VertexBuffer::CreateInfo vertexInfo{};
        vertexInfo.data = (uint8_t*)vertices;
        vertexInfo.size = sizeof(vertices);
        vertexInfo.layout = {};
        vertexInfo.layout.push(BufferLayout::Element::Type::VEC3, "iVertex");
        vertexInfo.layout.push(BufferLayout::Element::Type::VEC3, "iNormal");
        vertexInfo.layout.push(BufferLayout::Element::Type::VEC2, "iUV");

        IndexBuffer::CreateInfo indexInfo{};
        indexInfo.data = (uint8_t*)indices;
        indexInfo.size = sizeof(indices);

        Mesh::CreateInfo info{};
        info.vertexBufferInfo = vertexInfo;
        info.indexBufferInfo = indexInfo;
        _meshes["atta::gfx::quad3"] = create<Mesh>(info);
    }
    // Cube
    {
        static float vertices[] = {
            // Position         Normal            UV
            // Back face
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // Top-right
            1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // Bottom-right
            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // Top-right
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
            -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // Top-left
            // Front face
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom-left
            1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // Bottom-right
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // Top-right
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // Top-right
            -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // Top-left
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom-left
            // Left face
            -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // Top-right
            -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // Top-left
            -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom-left
            -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // Bottom-right
            -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // Top-right
            // Right face
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // Top-left
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom-right
            1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // Top-right
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom-right
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // Top-left
            1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // Bottom-left
            // Bottom face
            -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // Top-right
            1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // Top-left
            1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // Bottom-left
            1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // Bottom-left
            -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // Bottom-right
            -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // Top-right
            // Top face
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Top-left
            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // Bottom-right
            1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // Top-right
            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // Bottom-right
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Top-left
            -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f   // Bottom-left
        };
        static uint32_t indices[6 * 6];
        for (size_t i = 0; i < 6 * 6; i++)
            indices[i] = i;
        VertexBuffer::CreateInfo vertexInfo{};
        vertexInfo.data = (uint8_t*)vertices;
        vertexInfo.size = sizeof(vertices);
        vertexInfo.layout = {};
        vertexInfo.layout.push(BufferLayout::Element::Type::VEC3, "iVertex");
        vertexInfo.layout.push(BufferLayout::Element::Type::VEC3, "iNormal");
        vertexInfo.layout.push(BufferLayout::Element::Type::VEC2, "iUV");

        IndexBuffer::CreateInfo indexInfo{};
        indexInfo.data = (uint8_t*)indices;
        indexInfo.size = sizeof(indices);

        Mesh::CreateInfo info{};
        info.vertexBufferInfo = vertexInfo;
        info.indexBufferInfo = indexInfo;
        _meshes["atta::gfx::cube"] = create<Mesh>(info);
    }
}

void Manager::onMeshLoadEvent(event::Event& event) {
    event::MeshLoad& e = reinterpret_cast<event::MeshLoad&>(event);
    createMesh(e.sid);
}

void Manager::onMeshUpdateEvent(event::Event& event) {
    event::MeshUpdate& e = reinterpret_cast<event::MeshUpdate&>(event);

    resource::Mesh* meshResource = resource::get<resource::Mesh>(e.sid.getString());
    if (_meshes.find(e.sid) != _meshes.end()) {
        std::shared_ptr<gfx::Mesh> mesh = _meshes[e.sid];
        mesh->getVertexBuffer()->update(meshResource->getVertices().data(), meshResource->getVertices().size());
    } else
        LOG_WARN("gfx::Manager", "Can not update mesh [w]$0[] that was not created", e.sid);
}

void Manager::onMeshDestroyEvent(event::Event& event) {
    event::MeshDestroy& e = reinterpret_cast<event::MeshDestroy&>(event);
    if (_meshes.find(e.sid) != _meshes.end())
        _meshes[e.sid].reset();
    else
        LOG_WARN("gfx::Manager", "Can not destroy mesh [w]$0[] that was not created", e.sid);
}

void Manager::onImageLoadEvent(event::Event& event) {
    event::ImageLoad& e = reinterpret_cast<event::ImageLoad&>(event);

    createImage(e.sid);
}

void Manager::onImageUpdateEvent(event::Event& event) {
    event::ImageUpdate& e = reinterpret_cast<event::ImageUpdate&>(event);

    // Get image resource
    resource::Image* imageRes = resource::get<resource::Image>(e.sid.getString());
    if (imageRes == nullptr) {
        LOG_WARN("gfx::Manager", "Could not update image from [w]$0[], image resource does not exists", e.sid.getString());
        return;
    }

    // Get image
    if (_images.find(e.sid) == _images.end()) {
        _images[e.sid]->write(resource::get<resource::Image>(e.sid.getString())->getData());
        LOG_WARN("gfx::Manager", "Could not update image [w]$0[] that does not exists", e.sid);
        return;
    }
    std::shared_ptr<gfx::Image> image = _images[e.sid];

    // Resize image if needed
    if (image->getWidth() != imageRes->getWidth() || image->getHeight() != imageRes->getHeight())
        image->resize(imageRes->getWidth(), imageRes->getHeight());

    // Update image data
    image->write(imageRes->getData());
}

void Manager::createMesh(StringId sid) {
    LOG_VERBOSE("gfx::Manager", "Create mesh [w]$0[]", sid);
    resource::Mesh* mesh = resource::get<resource::Mesh>(sid.getString());

    VertexBuffer::CreateInfo vertexInfo{};
    vertexInfo.data = (uint8_t*)mesh->getVertices().data();
    vertexInfo.size = mesh->getVertices().size();
    // Populate vertex layout
    for (resource::Mesh::VertexElement element : mesh->getVertexLayout()) {
        BufferLayout::Element::Type type;
        switch (element.type) {
            case resource::Mesh::VertexElement::FLOAT:
                type = BufferLayout::Element::Type::FLOAT;
                break;
            case resource::Mesh::VertexElement::VEC2:
                type = BufferLayout::Element::Type::VEC2;
                break;
            case resource::Mesh::VertexElement::VEC3:
                type = BufferLayout::Element::Type::VEC3;
                break;
            case resource::Mesh::VertexElement::VEC4:
                type = BufferLayout::Element::Type::VEC4;
                break;
        }
        vertexInfo.layout.push(type, element.name);
    }

    IndexBuffer::CreateInfo indexInfo{};
    if (!mesh->getIndices().empty()) {
        indexInfo.data = (uint8_t*)mesh->getIndices().data();
        indexInfo.size = mesh->getIndices().size() * sizeof(res::Mesh::Index);
    }

    Mesh::CreateInfo info{};
    info.vertexBufferInfo = vertexInfo;
    info.indexBufferInfo = indexInfo;
    _meshes[sid] = create<Mesh>(info);
}

void Manager::createImage(StringId sid) {
    LOG_VERBOSE("gfx::Manager", "Create image [w]$0[]", sid);
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

const std::unordered_map<StringId, std::shared_ptr<Mesh>>& Manager::getMeshes() const { return _meshes; }
const std::unordered_map<StringId, std::shared_ptr<Image>>& Manager::getImages() const { return _images; }

//---------- Register API specific implementations ----------//
// For each type, will return the OpenGL, Vulkan, ... implementation based on the current active graphicsAPI
// If the derived (e.g. VulkanImage) has the same type as the base (e.g. Image), it means that does not exists
// an implementation of Image for vulkan

#if ATTA_VULKAN_SUPPORT
// Vulkan supported
#define CHECK_VK_SUPPORT(x) vk::x
#else
// Vulkan not supported
#define CHECK_VK_SUPPORT(x) x
#endif

template <>
std::shared_ptr<Image> Manager::createImpl<Image>(Image::CreateInfo info) {
    return createSpecific<Image, gl::Image, CHECK_VK_SUPPORT(Image)>(info);
}

template <>
std::shared_ptr<Mesh> Manager::createImpl<Mesh>(Mesh::CreateInfo info) {
    return createSpecific<Mesh, gl::Mesh, CHECK_VK_SUPPORT(Mesh)>(info);
}

template <>
std::shared_ptr<Framebuffer> Manager::createImpl<Framebuffer>(Framebuffer::CreateInfo info) {
    return createSpecific<Framebuffer, gl::Framebuffer, CHECK_VK_SUPPORT(Framebuffer)>(info);
}

template <>
std::shared_ptr<VertexBuffer> Manager::createImpl<VertexBuffer>(VertexBuffer::CreateInfo info) {
    return createSpecific<VertexBuffer, gl::VertexBuffer, CHECK_VK_SUPPORT(VertexBuffer)>(info);
}

template <>
std::shared_ptr<IndexBuffer> Manager::createImpl<IndexBuffer>(IndexBuffer::CreateInfo info) {
    return createSpecific<IndexBuffer, gl::IndexBuffer, CHECK_VK_SUPPORT(IndexBuffer)>(info);
}

template <>
std::shared_ptr<RenderPass> Manager::createImpl<RenderPass>(RenderPass::CreateInfo info) {
    return createSpecific<RenderPass, gl::RenderPass, CHECK_VK_SUPPORT(RenderPass)>(info);
}

template <>
std::shared_ptr<Shader> Manager::createImpl<Shader>(const char* file) {
    return createImpl<Shader>(fs::path(file));
}

template <>
std::shared_ptr<Shader> Manager::createImpl<Shader>(fs::path file) {
    return createSpecific<Shader, gl::Shader, CHECK_VK_SUPPORT(Shader)>(file);
}

template <>
std::shared_ptr<Pipeline> Manager::createImpl<Pipeline>(Pipeline::CreateInfo info) {
    return createSpecific<Pipeline, gl::Pipeline, CHECK_VK_SUPPORT(Pipeline)>(info);
}

template <>
std::shared_ptr<RenderQueue> Manager::createImpl<RenderQueue>() {
    return createSpecific<RenderQueue, gl::RenderQueue, CHECK_VK_SUPPORT(RenderQueue)>();
}

} // namespace atta::graphics
