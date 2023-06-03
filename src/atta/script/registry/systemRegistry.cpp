//--------------------------------------------------
// Atta Script Module
// systemRegistry.cpp
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/script/registry/systemRegistry.h>

namespace atta::script {

SystemRegistry::SystemRegistry(std::string name, std::string typeidName, size_t typeidHash) : Registry(name, typeidName, typeidHash) {}

const std::vector<SystemRegistry*>& SystemRegistry::getRegistries() {
    auto& regs = getRegs();
    return regs;
}

void SystemRegistry::addRegistry(SystemRegistry* registry) {
    LOG_DEBUG("scr::Registry", "Add system registry [w]$0", *registry);
    auto& regs = getRegs();
    regs.push_back(registry);
}

void SystemRegistry::removeRegistry(SystemRegistry* registry) {
    auto& regs = getRegs();
    LOG_DEBUG("scr::Registry", "Remove system registry [w]$0", *registry);
    for (size_t i = 0; i < regs.size(); i++)
        if (regs[i] == registry) {
            regs.erase(regs.begin() + i);
            break;
        }
}

std::vector<SystemRegistry*>& SystemRegistry::getRegs() {
    static std::vector<SystemRegistry*> registries = {};
    return registries;
}

void SystemRegistry::onLoad() {
    for (SystemRegistry* r : getRegs())
        r->runOnLoad();
}

void SystemRegistry::onUnload() {
    for (SystemRegistry* r : getRegs())
        r->runOnUnload();
}

void SystemRegistry::onUIRender() {
    for (SystemRegistry* r : getRegs())
        r->runOnUIRender();
}

void SystemRegistry::onAttaLoop() {
    for (SystemRegistry* r : getRegs())
        r->runOnAttaLoop();
}

} // namespace atta::script
