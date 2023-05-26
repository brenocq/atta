//--------------------------------------------------
// Atta Script Module
// worldRegistry.cpp
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/script/registry/worldRegistry.h>

namespace atta::script {

WorldRegistry::WorldRegistry(std::string name, std::string typeidName, size_t typeidHash) : Registry(name, typeidName, typeidHash) {}

const std::vector<WorldRegistry*>& WorldRegistry::getRegistries() {
    auto& regs = getRegs();
    return regs;
}

void WorldRegistry::addRegistry(WorldRegistry* registry) {
    LOG_DEBUG("scr::Registry", "Add world registry [w]$0", *registry);
    auto& regs = getRegs();
    regs.push_back(registry);
}

void WorldRegistry::removeRegistry(WorldRegistry* registry) {
    auto& regs = getRegs();
    LOG_DEBUG("scr::Registry", "Remove world registry [w]$0", *registry);
    for (size_t i = 0; i < regs.size(); i++)
        if (regs[i] == registry) {
            regs.erase(regs.begin() + i);
            break;
        }
}

std::vector<WorldRegistry*>& WorldRegistry::getRegs() {
    static std::vector<WorldRegistry*> registries = {};
    return registries;
}

void WorldRegistry::onLoad() {
    for (WorldRegistry* r : getRegs())
        r->runOnLoad();
}

void WorldRegistry::onUnload() {
    for (WorldRegistry* r : getRegs())
        r->runOnUnload();
}

void WorldRegistry::onStart() {
    for (WorldRegistry* r : getRegs())
        r->runOnStart();
}

void WorldRegistry::onStop() {
    for (WorldRegistry* r : getRegs())
        r->runOnStop();
}

void WorldRegistry::onContinue() {
    for (WorldRegistry* r : getRegs())
        r->runOnContinue();
}

void WorldRegistry::onPause() {
    for (WorldRegistry* r : getRegs())
        r->runOnPause();
}

void WorldRegistry::onUpdateBefore(float dt) {
    for (WorldRegistry* r : getRegs())
        r->runOnUpdateBefore(dt);
}

void WorldRegistry::onUpdateAfter(float dt) {
    for (WorldRegistry* r : getRegs())
        r->runOnUpdateAfter(dt);
}

void WorldRegistry::onUIRender() {
    for (WorldRegistry* r : getRegs())
        r->runOnUIRender();
}

void WorldRegistry::onAttaLoop() {
    for (WorldRegistry* r : getRegs())
        r->runOnAttaLoop();
}

} // namespace atta::script
