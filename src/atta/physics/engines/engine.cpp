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

void Engine::createRigidBody(component::Entity entity) {}
void Engine::deleteRigidBody(component::Entity entity) {}
void Engine::createColliders(component::Entity entity) {}
void Engine::deleteColliders(component::Entity entity) {}

std::vector<component::Entity> Engine::getEntityCollisions(component::Entity entity) { return {}; }
std::vector<RayCastHit> Engine::rayCast(vec3 begin, vec3 end, bool onlyFirst) { return {}; }
bool Engine::areColliding(component::Entity entity0, component::Entity entity1) { return false; }

} // namespace atta::physics
