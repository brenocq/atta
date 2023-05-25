//--------------------------------------------------
// Atta Script Module
// registry.cpp
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/script/registry/registry.h>

namespace atta::script {

Registry::Registry(std::string typeidName, size_t typeidHash) : _typeidName(typeidName), _typeidHash(typeidHash) { _name = _typeidName; }

std::string Registry::getName() const { return _name; }

std::string Registry::getTypeidName() const { return _typeidName; }

size_t Registry::getTypeidHash() const { return _typeidHash; }

const std::vector<Registry*>& Registry::getRegistries() { return _registries; }

void Registry::addRegistry(Registry* registry) {
    LOG_DEBUG("scr::Registry", "Add registry [w]$0", *registry);
    _registries.push_back(registry);
}

void Registry::removeRegistry(Registry* registry) {
    LOG_DEBUG("scr::Registry", "Remove registry [w]$0", *registry);
    for (size_t i = 0; i < _registries.size(); i++)
        if (_registries[i] == registry) {
            _registries.erase(_registries.begin() + 1);
            break;
        }
}

} // namespace atta::script
