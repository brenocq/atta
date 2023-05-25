//--------------------------------------------------
// Atta Script Module
// worldRegistry.cpp
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/script/registry/worldRegistry.h>

namespace atta::script {

std::vector<WorldRegistry*> WorldRegistry::_registries = {};

WorldRegistry::WorldRegistry(std::string name, std::string typeidName, size_t typeidHash) : Registry(name, typeidName, typeidHash) {}

const std::vector<WorldRegistry*>& WorldRegistry::getRegistries() { return _registries; }

void WorldRegistry::addRegistry(WorldRegistry* registry) {
    LOG_DEBUG("scr::Registry", "Add world registry [w]$0", *registry);
    _registries.push_back(registry);
}

void WorldRegistry::removeRegistry(WorldRegistry* registry) {
    LOG_DEBUG("scr::Registry", "Remove world registry [w]$0", *registry);
    for (size_t i = 0; i < _registries.size(); i++)
        if (_registries[i] == registry) {
            _registries.erase(_registries.begin() + 1);
            break;
        }
}

void WorldRegistry::onLoad() {
    for (WorldRegistry* r : _registries)
        r->runOnLoad();
}

void WorldRegistry::onUnload() {
    for (WorldRegistry* r : _registries)
        r->runOnUnload();
}

void WorldRegistry::onStart() {
    for (WorldRegistry* r : _registries)
        r->runOnStart();
}

void WorldRegistry::onStop() {
    for (WorldRegistry* r : _registries)
        r->runOnStop();
}

void WorldRegistry::onContinue() {
    for (WorldRegistry* r : _registries)
        r->runOnContinue();
}

void WorldRegistry::onPause() {
    for (WorldRegistry* r : _registries)
        r->runOnPause();
}

void WorldRegistry::onUpdateBefore(float dt) {
    for (WorldRegistry* r : _registries)
        r->runOnUpdateBefore(dt);
}

void WorldRegistry::onUpdateAfter(float dt) {
    for (WorldRegistry* r : _registries)
        r->runOnUpdateAfter(dt);
}

void WorldRegistry::onUIRender() {
    for (WorldRegistry* r : _registries)
        r->runOnUIRender();
}

void WorldRegistry::onAttaLoop() {
    for (WorldRegistry* r : _registries)
        r->runOnAttaLoop();
}

} // namespace atta::script
