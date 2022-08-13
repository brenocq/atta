//--------------------------------------------------
// Atta Resource Module
// interface.h
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RESOURCE_INTERFACE_H
#define ATTA_RESOURCE_INTERFACE_H

#include <atta/utils/stringId.h>

namespace atta::resource {

void startUp();
void shutDown();

template <typename R, typename... Args>
R* create(const fs::path& filename, Args... args);
template <typename R>
void destroy(const fs::path& filename);
template <typename R>
R* get(const fs::path& filename);
template <typename R>
std::vector<StringId> getResources();

} // namespace atta::resource

#include <atta/resource/manager.h>

namespace atta::resource {

template <typename R, typename... Args>
R* create(const fs::path& filename, Args... args) {
    return Manager::getInstance().createImpl<R>(filename, args...);
}
template <typename R>
void destroy(const fs::path& filename) {
    Manager::getInstance().destroyImpl<R>(filename);
}
template <typename R>
R* get(const fs::path& filename) {
    return Manager::getInstance().getImpl<R>(filename);
}
template <typename R>
std::vector<StringId> getResources() {
    return Manager::getInstance().getResourcesImpl<R>();
}

} // namespace atta::resource

#endif // ATTA_RESOURCE_INTERFACE_H
