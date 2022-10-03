//--------------------------------------------------
// Atta Physics Module
// interface.cpp
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/physics/interface.h>

namespace atta::physics {

void startUp() { Manager::getInstance().startUpImpl(); }
void shutDown() { Manager::getInstance().shutDownImpl(); }
void update(float dt) { Manager::getInstance().updateImpl(dt); }

Engine::Type getEngineType() { return Manager::getInstance()._engine->getType(); }
void setEngineType(Engine::Type type) { Manager::getInstance().setEngineTypeImpl(type); }

Plane2D getPlane2D() { return Manager::getInstance()._plane2D; }
void setPlane2D(Plane2D plane2D) { Manager::getInstance().setPlane2DImpl(plane2D); }
vec3 getGravity() { return Manager::getInstance()._gravity; }
void setGravity(vec3 gravity) { Manager::getInstance().setGravityImpl(gravity); }
bool getShowColliders() { return Manager::getInstance()._showColliders; }
void setShowColliders(bool showColliders) { Manager::getInstance()._showColliders = showColliders; }
bool getShowContacts() { return Manager::getInstance()._showContacts; }
void setShowContacts(bool showContacts) { Manager::getInstance()._showContacts = showContacts; }
bool getShowJoints() { return Manager::getInstance()._showJoints; }
void setShowJoints(bool showJoints) { Manager::getInstance()._showJoints = showJoints; }

//---------- Queries ----------//
std::vector<component::EntityId> getEntityCollisions(component::EntityId eid) { return Manager::getInstance()._engine->getEntityCollisions(eid); }
std::vector<component::EntityId> rayCast(vec3 begin, vec3 end, bool onlyFirst) {
    return Manager::getInstance()._engine->rayCast(begin, end, onlyFirst);
}
bool areColliding(component::EntityId eid0, component::EntityId eid1) { return Manager::getInstance()._engine->areColliding(eid0, eid1); }

} // namespace atta::physics
