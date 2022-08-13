//--------------------------------------------------
// Atta Resource Module
// manager.cpp
// Date: 2021-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/resource/interface.h>
#include <atta/resource/manager.h>

#include <atta/event/events/imageLoad.h>
#include <atta/event/events/materialLoad.h>
#include <atta/event/events/meshLoad.h>
#include <atta/event/events/projectOpen.h>
#include <atta/file/manager.h>
#include <atta/memory/interface.h>

namespace atta::resource {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::startUpImpl() {
    //----- Module Memory -----//
    // Get main memory
    memory::Allocator* mainAllocator = memory::getAllocator(SSID("MainAllocator"));
    size_t size = 128 * 1024 * 1024; // 128MB
    // Alloc memory inside main memory
    uint8_t* resourceMemory = static_cast<uint8_t*>(mainAllocator->allocBytes(size, sizeof(uint8_t)));
    _allocator = new memory::BitmapAllocator(resourceMemory, size);
    memory::registerAllocator(SSID("ResourceAllocator"), static_cast<memory::Allocator*>(_allocator));

    // Subscribe to project open (load project events when opened)
    event::subscribe<event::ProjectOpen>(BIND_EVENT_FUNC(Manager::onProjectOpen));

    // Default resources
    for (auto& resourcePath : file::getResourcePaths())
        loadResourcesRecursively(resourcePath);
}

void Manager::shutDownImpl() {}

void Manager::onProjectOpen(event::Event& event) {
    for (auto& resourcePath : file::getResourcePaths())
        loadResourcesRecursively(resourcePath);
}

void Manager::loadResourcesRecursively(fs::path directory) {
    static const std::vector<std::string> meshExtensions{".obj", ".fbx", ".FBX", ".fbx"}; //, ".3ds", ".stl", ".ply" };
    static const std::vector<std::string> imageExtensions{".jpg", ".jpeg", ".png", ".hdr", ".tga"};

    for (auto file : file::getDirectoryFilesRecursive(directory)) {
        // Load as meshe
        for (auto& extension : meshExtensions)
            if (extension == file.extension().string()) {
                LOG_DEBUG("resource::Manager", "Resource mesh: [w]$0[]", file.string());
                resource::get<Mesh>(file.string());
                break;
            }
        // Load as image
        for (auto& extension : imageExtensions)
            if (extension == file.extension().string()) {
                LOG_DEBUG("resource::Manager", "Resource image: [w]$0[]", file.string());
                resource::get<Image>(file.string());
                break;
            }
    }
}

template <>
void Manager::createLoadEvent<Mesh>(Mesh* resource, StringId sid) {
    event::MeshLoad e(sid);
    event::publish(e);
}
template <>
void Manager::createLoadEvent<Image>(Image* resource, StringId sid) {
    event::ImageLoad e(sid);
    event::publish(e);
}
template <>
void Manager::createLoadEvent<Material>(Material* resource, StringId sid) {
    event::MaterialLoad e(sid);
    event::publish(e);
}

} // namespace atta::resource
