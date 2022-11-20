//--------------------------------------------------
// Atta Physics Module
// physicsEngine.cpp
// Date: 2021-11-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/physics/engines/engine.h>

namespace atta::physics {

Engine::Engine(Type type) : _type(type), _running(false) {}

} // namespace atta::physics
