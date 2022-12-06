//--------------------------------------------------
// Atta Physics Module
// physicsEngine.cpp
// Date: 2021-11-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/physics/engines/engine.h>
#include <atta/physics/interface.h>

namespace atta::physics {

Engine::Engine(Type type) : _type(type), _running(false) {}

std::vector<component::EntityId> Engine::getEntityCollisions(component::EntityId eid) { return {}; }
std::vector<RayCastHit> Engine::rayCast(vec3 begin, vec3 end, bool onlyFirst) { return {}; }
bool Engine::areColliding(component::EntityId eid0, component::EntityId eid1) { return false; }

} // namespace atta::physics
