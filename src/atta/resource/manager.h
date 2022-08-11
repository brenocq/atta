//--------------------------------------------------
// Atta Resource Module
// manager.h
// Date: 2021-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RESOURCE_RESOURCE_MANAGER_H
#define ATTA_RESOURCE_RESOURCE_MANAGER_H

#include <atta/event/manager.h>
#include <atta/memory/allocators/bitmapAllocator.h>
#include <atta/resource/resources/resources.h>

namespace atta::resource {

class Manager final {
  public:
    using ResourceType = size_t;

    static Manager& getInstance();

    friend void startUp();
    friend void shutDown();
    template <typename R, typename... Args>
    friend R* create(const fs::path& filename, Args... args);
    template <typename R>
    friend R* get(const fs::path& filename);
    template <typename R>
    friend std::vector<StringId> getResources();

  private:
    void startUpImpl();
    void shutDownImpl();
    void loadResourcesRecursively(fs::path directory);
    void onProjectOpen(event::Event& event);

    template <typename R, typename... Args>
    R* createImpl(const fs::path& filename, Args... args);
    template <typename R>
    R* getImpl(const fs::path& filename);
    template <typename R>
    std::vector<StringId> getResourcesImpl();

    template <typename R>
    void createLoadEvent(R* resource, StringId sid);

    memory::BitmapAllocator* _allocator;
    std::unordered_map<StringHash, uint8_t*> _resourceMap;
    std::unordered_map<ResourceType, std::vector<StringId>> _resourcesByType;
};

} // namespace atta::resource

#include <atta/resource/manager.inl>

#endif // ATTA_RESOURCE_RESOURCE_MANAGER_H
