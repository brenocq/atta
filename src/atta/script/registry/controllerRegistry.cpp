//--------------------------------------------------
// Atta Script Module
// controllerRegistry.cpp
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/script/registry/controllerRegistry.h>

namespace atta::script {

ControllerRegistry::ControllerRegistry(std::string name, std::string typeidName, size_t typeidHash) : Registry(name, typeidName, typeidHash) {}

const std::vector<const ControllerRegistry*>& ControllerRegistry::getRegistries() {
    auto& regs = getRegs();
    return regs;
}

const ControllerRegistry* ControllerRegistry::getRegistry(StringId sid) {
    std::string scriptName = sid.getString();
    auto& regs = getRegs();
    for (const ControllerRegistry* r : regs)
        if (r->getName() == scriptName)
            return r;
    return nullptr;
}

void ControllerRegistry::addRegistry(const ControllerRegistry* registry) {
    auto& regs = getRegs();
    LOG_DEBUG("scr::Registry", "Add controller registry [w]$0", *registry);
    regs.push_back(registry);
    regs = getRegs();
}

void ControllerRegistry::removeRegistry(const ControllerRegistry* registry) {
    auto& regs = getRegs();
    LOG_DEBUG("scr::Registry", "Remove controller registry [w]$0", *registry);
    for (size_t i = 0; i < regs.size(); i++)
        if (regs[i] == registry) {
            regs.erase(regs.begin() + i);
            break;
        }
    getRegs();
}

std::vector<const ControllerRegistry*>& ControllerRegistry::getRegs() {
    static std::vector<const ControllerRegistry*> registries = {};
    return registries;
}

} // namespace atta::script
