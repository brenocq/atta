//--------------------------------------------------
// Atta Resource Module
// manager.inl
// Date: 2021-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta::resource {

template <typename R, typename... Args>
R* Manager::createImpl(const fs::path& filename, Args... args) {
    StringId sid = StringId(filename.string());

    // Create resource, potentially overwriting the already loaded
    R* resource = new R(filename, args...);
    if (resource == nullptr) {
        LOG_ERROR("resource::Manager", "Could not create resource [*w]$0[] ([w]$1[]). Probably out of memory", sid, typeid(R).name());
        return nullptr;
    }
    _resourceMap[sid.getId()] = reinterpret_cast<uint8_t*>(resource);
    _resourcesByType[typeid(R).hash_code()].push_back(sid);

    // Create resource load event
    createLoadEvent<R>(resource, sid);

    return resource;
}

template <typename R>
void Manager::destroyImpl(const fs::path& filename) {
    StringId sid = StringId(filename.string());
    if (_resourceMap.find(sid.getId()) != _resourceMap.end()) {
        R* r = reinterpret_cast<R*>(_resourceMap[sid.getId()]);
        delete r;
        // Remove sid from map
        _resourceMap.erase(sid.getId());
        // Remove sid from vector
        std::vector<StringId> resources;
        for(StringId rsid : _resourcesByType[typeid(R).hash_code()])
            if(rsid != sid)
                resources.push_back(rsid);
        _resourcesByType[typeid(R).hash_code()] = resources;
    }
}

template <typename R>
R* Manager::getImpl(const fs::path& filename) {
    StringId sid = StringId(filename.string());
    if (_resourceMap.find(sid.getId()) == _resourceMap.end()) {
        // Create resource and call load event
        R* resource = createImpl<R>(filename);
        return resource;
    } else {
        // Return loaded resource
        return reinterpret_cast<R*>(_resourceMap[sid.getId()]);
    }
}

template <typename R>
void Manager::createLoadEvent(R* resource, StringId sid) {
    LOG_ERROR("resource::Manager", "Could create load event for resource [*w]$0[] ([w]$1[]). It is a resource?", sid, typeid(R).name());
}
template <>
void Manager::createLoadEvent<Mesh>(Mesh* resource, StringId sid);
template <>
void Manager::createLoadEvent<Image>(Image* resource, StringId sid);
template <>
void Manager::createLoadEvent<Material>(Material* resource, StringId sid);

template <typename R>
void Manager::destroyResourcesImpl() {
    for(StringId sid : getResources<R>())
    {
        delete reinterpret_cast<R*>(_resourceMap[sid.getId()]);
        _resourceMap.erase(sid.getId());
    }
    _resourcesByType.erase(typeid(R).hash_code());
}

template <typename R>
std::vector<StringId> Manager::getResourcesImpl() {
    return _resourcesByType[typeid(R).hash_code()];
}

} // namespace atta::resource
