//--------------------------------------------------
// Atta Script Module
// controllerRegistry.cpp
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/script/registry/controllerRegistry.h>

namespace atta::script {

std::vector<ControllerRegistry*> ControllerRegistry::_registries = {};

ControllerRegistry::ControllerRegistry(std::string name, std::string typeidName, size_t typeidHash) : Registry(name, typeidName, typeidHash) {}

const std::vector<ControllerRegistry*>& ControllerRegistry::getRegistries() { return _registries; }

ControllerRegistry* ControllerRegistry::getRegistry(StringId sid) {
    std::string scriptName = sid.getString();
    for(ControllerRegistry* r : _registries)
        if(r->getName() == scriptName)
            return r;
        return nullptr;
}

void ControllerRegistry::addRegistry(ControllerRegistry* registry) {
    LOG_DEBUG("scr::Registry", "Add controller registry [w]$0", *registry);
    _registries.push_back(registry);
}

void ControllerRegistry::removeRegistry(ControllerRegistry* registry) {
    LOG_DEBUG("scr::Registry", "Remove controller registry [w]$0", *registry);
    for (size_t i = 0; i < _registries.size(); i++)
        if (_registries[i] == registry) {
            _registries.erase(_registries.begin() + 1);
            break;
        }
}

} // namespace atta::script
