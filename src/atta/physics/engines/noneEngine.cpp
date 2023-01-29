//--------------------------------------------------
// Atta Physics Module
// noneEngine.cpp
// Date: 2022-09-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/physics/engines/noneEngine.h>
#include <atta/physics/interface.h>

namespace atta::physics {

NoneEngine::NoneEngine() : Engine(Engine::NONE) {}

NoneEngine::~NoneEngine() {
    if (_running)
        stop();
}

void NoneEngine::start() { _running = true; }

void NoneEngine::step(float dt) {}

void NoneEngine::stop() { _running = false; }

} // namespace atta::physics
