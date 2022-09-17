//--------------------------------------------------
// Atta Physics Module
// interface.h
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_INTERFACE_H
#define ATTA_PHYSICS_INTERFACE_H

#include <atta/physics/physicsEngines/physicsEngine.h>

namespace atta::physics {

enum class Plane2D { X = 0, Y, Z };

void startUp();
void shutDown();
void update(float dt);

PhysicsEngine::Type getSelectedEngine();
Plane2D getPlane2D();
void setSelectedEngine(PhysicsEngine::Type type);
std::shared_ptr<PhysicsEngine> getPhysicsEngine();

//---------- Queries ----------//
std::vector<component::EntityId> getEntityCollisions(component::EntityId eid);
std::vector<component::EntityId> rayCast(vec3 begin, vec3 end, bool onlyFirst = true);
bool areColliding(component::EntityId eid0, component::EntityId eid1);

} // namespace atta::physics

#endif // ATTA_PHYSICS_INTERFACE_H