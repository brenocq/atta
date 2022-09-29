//--------------------------------------------------
// Atta Physics Module
// noneEngine.cpp
// Date: 2022-09-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/physics/engines/noneEngine.h>

namespace atta::physics {

NoneEngine::NoneEngine() : Engine(Engine::NONE) {}

NoneEngine::~NoneEngine() {
    if (_running)
        stop();
}

void NoneEngine::start() { _running = true; }

void NoneEngine::step(float dt) {}

void NoneEngine::stop() { _running = false; }

std::vector<component::EntityId> NoneEngine::getEntityCollisions(component::EntityId eid) { return {}; }

std::vector<component::EntityId> NoneEngine::rayCast(vec3 begin, vec3 end, bool onlyFirst) { return {}; }

bool NoneEngine::areColliding(component::EntityId eid0, component::EntityId eid1) { return false; }

} // namespace atta::physics
