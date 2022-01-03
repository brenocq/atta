//--------------------------------------------------
// Atta Resource System
// resourceManager.cpp
// Date: 2021-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/resourceSystem/resourceManager.h>
#include <atta/memorySystem/memoryManager.h>
#include <atta/eventSystem/events/meshLoadEvent.h>
#include <atta/eventSystem/events/textureLoadEvent.h>
#include <atta/eventSystem/events/projectOpenEvent.h>
#include <atta/fileSystem/fileManager.h>

namespace atta
{
    ResourceManager& ResourceManager::getInstance()
    {
        static ResourceManager instance;
        return instance;
    }

    void ResourceManager::startUp() { getInstance().startUpImpl(); }
    void ResourceManager::startUpImpl()
    {
        //----- System Memory -----//
        // Get main memory
        Allocator* mainAllocator = MemoryManager::getAllocator(SSID("MainAllocator"));
        size_t size = 512*1024*1024;// 512MB
        // Alloc memory inside main memory
        uint8_t* resourceMemory = static_cast<uint8_t*>(mainAllocator->allocBytes(size, sizeof(uint8_t)));
        _allocator = new BitmapAllocator(resourceMemory, size);
        MemoryManager::registerAllocator(SSID("ResourceAllocator"), static_cast<Allocator*>(_allocator));

        // Subscribe to project open (load project events when opened)
        EventManager::subscribe<ProjectOpenEvent>(BIND_EVENT_FUNC(ResourceManager::onProjectOpen));

        // Default resources
        for(auto& resourcePath : FileManager::getResourcePaths())
            loadResourcesRecursively(resourcePath);
    }

    void ResourceManager::shutDown() { getInstance().shutDownImpl(); }
    void ResourceManager::shutDownImpl()
    {

    }

    void ResourceManager::onProjectOpen(Event& event)
    {
        for(auto& resourcePath : FileManager::getResourcePaths())
            loadResourcesRecursively(resourcePath);
    }

    void ResourceManager::loadResourcesRecursively(fs::path directory)
    {
        static const std::vector<std::string> meshExtensions { ".obj", ".fbx", ".FBX", ".fbx" };//, ".3ds", ".stl", ".ply" };
        static const std::vector<std::string> textureExtensions { ".jpg", ".jpeg", ".png", ".hdr", ".tga" };

        for(auto file : FileManager::getDirectoryFilesRecursive(directory))
        {
            // Load as meshe
            for(auto& extension : meshExtensions)
                if(extension == file.extension().string())
                {
                    LOG_DEBUG("ResourceManager", "Resource mesh: [w]$0[]", file.string());
                    ResourceManager::get<Mesh>(file.string());
                    break;
                }
            // Load as texture
            for(auto& extension : textureExtensions)
                if(extension == file.extension().string())
                {
                    LOG_DEBUG("ResourceManager", "Resource texture: [w]$0[]", file.string());
                    ResourceManager::get<Texture>(file.string());
                    break;
                }
        }
    }

    template <>
    void ResourceManager::createLoadEvent<Mesh>(Mesh* resource, StringId sid)
    {
        MeshLoadEvent e(sid);
        EventManager::publish(e);
    }
    template <>
    void ResourceManager::createLoadEvent<Texture>(Texture* resource, StringId sid)
    {
        TextureLoadEvent e(sid);
        EventManager::publish(e);
    }
}
