//--------------------------------------------------
// Atta Physics Module
// interface.cpp
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/physics/interface.h>
#include <atta/physics/manager.h>

namespace atta::physics {

void startUp() { Manager::getInstance().startUpImpl(); }
void shutDown() { Manager::getInstance().shutDownImpl(); }
void update(float dt) { Manager::getInstance().updateImpl(dt); }

Engine::Type getSelectedEngine() { return Manager::getInstance()._engine->getType(); }
Plane2D getPlane2D() { return Manager::getInstance()._plane2D; }
void setSelectedEngine(Engine::Type type) { return Manager::getInstance().setSelectedEngineImpl(type); }
std::shared_ptr<Engine> getEngine() { return Manager::getInstance()._engine; }

//---------- Queries ----------//
std::vector<component::EntityId> getEntityCollisions(component::EntityId eid) { return Manager::getInstance()._engine->getEntityCollisions(eid); }
std::vector<component::EntityId> rayCast(vec3 begin, vec3 end, bool onlyFirst) {
    return Manager::getInstance()._engine->rayCast(begin, end, onlyFirst);
}
bool areColliding(component::EntityId eid0, component::EntityId eid1) { return Manager::getInstance()._engine->areColliding(eid0, eid1); }

} // namespace atta::physics
