//--------------------------------------------------
// Atta Resource Module
// manager.cpp
// Date: 2021-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/event/events/meshLoadEvent.h>
#include <atta/event/events/projectOpenEvent.h>
#include <atta/event/events/textureLoadEvent.h>
#include <atta/file/fileManager.h>
#include <atta/memory/memoryManager.h>
#include <atta/resource/resourceManager.h>

namespace atta::resource {
Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::startUp() { getInstance().startUpImpl(); }
void Manager::startUpImpl() {
    //----- Module Memory -----//
    // Get main memory
    Allocator* mainAllocator = MemoryManager::getAllocator(SSID("MainAllocator"));
    size_t size = 128 * 1024 * 1024; // 128MB
    // Alloc memory inside main memory
    uint8_t* resourceMemory = static_cast<uint8_t*>(mainAllocator->allocBytes(size, sizeof(uint8_t)));
    _allocator = new BitmapAllocator(resourceMemory, size);
    MemoryManager::registerAllocator(SSID("ResourceAllocator"), static_cast<Allocator*>(_allocator));

    // Subscribe to project open (load project events when opened)
    EventManager::subscribe<ProjectOpenEvent>(BIND_EVENT_FUNC(Manager::onProjectOpen));

    // Default resources
    for (auto& resourcePath : FileManager::getResourcePaths())
        loadResourcesRecursively(resourcePath);
}

void Manager::shutDown() { getInstance().shutDownImpl(); }
void Manager::shutDownImpl() {}

void Manager::onProjectOpen(Event& event) {
    for (auto& resourcePath : FileManager::getResourcePaths())
        loadResourcesRecursively(resourcePath);
}

void Manager::loadResourcesRecursively(fs::path directory) {
    static const std::vector<std::string> meshExtensions{".obj", ".fbx", ".FBX", ".fbx"}; //, ".3ds", ".stl", ".ply" };
    static const std::vector<std::string> textureExtensions{".jpg", ".jpeg", ".png", ".hdr", ".tga"};

    for (auto file : FileManager::getDirectoryFilesRecursive(directory)) {
        // Load as meshe
        for (auto& extension : meshExtensions)
            if (extension == file.extension().string()) {
                LOG_DEBUG("rsc::Manager", "Resource mesh: [w]$0[]", file.string());
                Manager::get<Mesh>(file.string());
                break;
            }
        // Load as texture
        for (auto& extension : textureExtensions)
            if (extension == file.extension().string()) {
                LOG_DEBUG("rsc::Manager", "Resource texture: [w]$0[]", file.string());
                Manager::get<Texture>(file.string());
                break;
            }
    }
}

template <>
void Manager::createLoadEvent<Mesh>(Mesh* resource, StringId sid) {
    MeshLoadEvent e(sid);
    EventManager::publish(e);
}
template <>
void Manager::createLoadEvent<Texture>(Texture* resource, StringId sid) {
    TextureLoadEvent e(sid);
    EventManager::publish(e);
}
} // namespace atta::resource
