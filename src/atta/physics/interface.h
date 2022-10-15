//--------------------------------------------------
// Atta Physics Module
// interface.h
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_INTERFACE_H
#define ATTA_PHYSICS_INTERFACE_H

#include <atta/physics/engines/engine.h>

namespace atta::physics {

enum class Plane2D { X = 0, Y, Z };

void startUp();
void shutDown();
void update(float dt);

Engine::Type getEngineType();
void setEngineType(Engine::Type type);
/// Get physics engine. If no engine type is specified, active engine is returned
template <typename T = Engine>
std::shared_ptr<T> getEngine();

Plane2D getPlane2D();
void getPlane2D(Plane2D plane2D);
vec3 getGravity();
void setGravity(vec3 gravity);
bool getShowColliders();
void setShowColliders(bool showColliders);
bool getShowContacts();
void setShowContacts(bool showContacts);
bool getShowJoints();
void setShowJoints(bool showJoints);

//---------- Queries ----------//
std::vector<component::EntityId> getEntityCollisions(component::EntityId eid);
std::vector<component::EntityId> rayCast(vec3 begin, vec3 end, bool onlyFirst = false);
bool areColliding(component::EntityId eid0, component::EntityId eid1);

} // namespace atta::physics

#include <atta/physics/manager.h>

namespace atta::physics {

template <typename T>
std::shared_ptr<T> getEngine() {
    if constexpr(std::is_same_v<T, Engine>)
        return Manager::getInstance()._engine;
    else if constexpr(std::is_same_v<T, NoneEngine>)
        return Manager::getInstance()._noneEngine;
    else if constexpr(std::is_same_v<T, BulletEngine>)
        return Manager::getInstance()._bulletEngine;
    else if constexpr(std::is_same_v<T, Box2DEngine>)
        return Manager::getInstance()._box2DEngine;
    return nullptr;
}

} // namespace atta::graphics

#endif // ATTA_PHYSICS_INTERFACE_H
