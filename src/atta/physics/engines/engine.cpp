//--------------------------------------------------
// Atta Physics Module
// physicsEngine.cpp
// Date: 2021-11-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/physics/engines/engine.h>
#include <atta/physics/interface.h>

namespace atta::physics {

// Map Type to String
const std::unordered_map<Engine::Type, std::string> Engine::typeToString = {
    {Engine::NONE, "NONE"},
    {Engine::BOX2D, "BOX2D"},
    {Engine::BULLET, "BULLET"},
};

// Map String to Type
const std::unordered_map<std::string, Engine::Type> Engine::stringToType = {
    {"NONE", Engine::NONE},
    {"BOX2D", Engine::BOX2D},
    {"BULLET", Engine::BULLET},
};

Engine::Engine(Type type) : _type(type), _running(false) {}

void Engine::createRigidBody(component::EntityId entity) {}
void Engine::deleteRigidBody(component::EntityId entity) {}
void Engine::createColliders(component::EntityId entity) {}
void Engine::deleteColliders(component::EntityId entity) {}

std::vector<component::EntityId> Engine::getEntityCollisions(component::EntityId eid) { return {}; }
std::vector<RayCastHit> Engine::rayCast(vec3 begin, vec3 end, bool onlyFirst) { return {}; }
bool Engine::areColliding(component::EntityId eid0, component::EntityId eid1) { return false; }

} // namespace atta::physics
