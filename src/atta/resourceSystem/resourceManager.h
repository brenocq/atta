//--------------------------------------------------
// Atta Resource System
// resourceManager.h
// Date: 2021-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RESOURCE_SYSTEM_RESOURCE_MANAGER_H
#define ATTA_RESOURCE_SYSTEM_RESOURCE_MANAGER_H
#include <atta/memorySystem/allocators/bitmapAllocator.h>
#include <atta/eventSystem/eventManager.h>
#include <atta/core/stringId.h>
#include <atta/resourceSystem/resources/resources.h>

namespace atta
{
    class ResourceManager final
    {
    public:
        using ResourceType = size_t;

        static ResourceManager& getInstance();
        static void startUp();
        static void shutDown();

        template <typename R, typename... Args>
        static R* create(const fs::path& filename, Args... args) { return getInstance().createImpl<R>(filename, args...); }
        template <typename R>
        static R* get(const fs::path& filename) { return getInstance().getImpl<R>(filename); }
        template <typename R>
        static std::vector<StringId> getResources() { return getInstance().getResourcesImpl<R>(); }

    private:
        void startUpImpl();
        void shutDownImpl();
        void loadResourcesRecursively(fs::path directory);
        void onProjectOpen(Event& event);

        template <typename R, typename... Args>
        R* createImpl(const fs::path& filename, Args... args);
        template <typename R>
        R* getImpl(const fs::path& filename);
        template <typename R>
        std::vector<StringId> getResourcesImpl();

        template <typename R>
        void createLoadEvent(R* resource, StringId sid);

        BitmapAllocator* _allocator;
        std::unordered_map<StringHash, uint8_t*> _resourceMap;
        std::unordered_map<ResourceType, std::vector<StringId>> _resourcesByType;
    };
}

#include <atta/resourceSystem/resourceManager.inl>
#endif// ATTA_RESOURCE_SYSTEM_RESOURCE_MANAGER_H
