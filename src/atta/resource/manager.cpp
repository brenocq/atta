//--------------------------------------------------
// Atta Resource Module
// manager.cpp
// Date: 2021-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/event/events/imageLoad.h>
#include <atta/event/events/meshLoad.h>
#include <atta/event/events/projectOpen.h>
#include <atta/file/manager.h>
#include <atta/memory/manager.h>
#include <atta/resource/manager.h>

namespace atta::resource {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::startUp() { getInstance().startUpImpl(); }
void Manager::startUpImpl() {
    //----- Module Memory -----//
    // Get main memory
    memory::Allocator* mainAllocator = memory::Manager::getAllocator(SSID("MainAllocator"));
    size_t size = 128 * 1024 * 1024; // 128MB
    // Alloc memory inside main memory
    uint8_t* resourceMemory = static_cast<uint8_t*>(mainAllocator->allocBytes(size, sizeof(uint8_t)));
    _allocator = new memory::BitmapAllocator(resourceMemory, size);
    memory::Manager::registerAllocator(SSID("ResourceAllocator"), static_cast<memory::Allocator*>(_allocator));

    // Subscribe to project open (load project events when opened)
    event::Manager::subscribe<event::ProjectOpen>(BIND_EVENT_FUNC(Manager::onProjectOpen));

    // Default resources
    for (auto& resourcePath : file::Manager::getResourcePaths())
        loadResourcesRecursively(resourcePath);
}

void Manager::shutDown() { getInstance().shutDownImpl(); }
void Manager::shutDownImpl() {}

void Manager::onProjectOpen(event::Event& event) {
    for (auto& resourcePath : file::Manager::getResourcePaths())
        loadResourcesRecursively(resourcePath);
}

void Manager::loadResourcesRecursively(fs::path directory) {
    static const std::vector<std::string> meshExtensions{".obj", ".fbx", ".FBX", ".fbx"}; //, ".3ds", ".stl", ".ply" };
    static const std::vector<std::string> imageExtensions{".jpg", ".jpeg", ".png", ".hdr", ".tga"};

    for (auto file : file::Manager::getDirectoryFilesRecursive(directory)) {
        // Load as meshe
        for (auto& extension : meshExtensions)
            if (extension == file.extension().string()) {
                LOG_DEBUG("resource::Manager", "Resource mesh: [w]$0[]", file.string());
                Manager::get<Mesh>(file.string());
                break;
            }
        // Load as image
        for (auto& extension : imageExtensions)
            if (extension == file.extension().string()) {
                LOG_DEBUG("resource::Manager", "Resource image: [w]$0[]", file.string());
                Manager::get<Image>(file.string());
                break;
            }
    }
}

template <>
void Manager::createLoadEvent<Mesh>(Mesh* resource, StringId sid) {
    event::MeshLoad e(sid);
    event::Manager::publish(e);
}
template <>
void Manager::createLoadEvent<Image>(Image* resource, StringId sid) {
    event::ImageLoad e(sid);
    event::Manager::publish(e);
}

} // namespace atta::resource
