//--------------------------------------------------
// Atta Physics Module
// manager.h
// Date: 2021-11-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_MANAGER_H
#define ATTA_PHYSICS_MANAGER_H

#include <atta/event/event.h>
#include <atta/physics/engines/box2DEngine.h>
#include <atta/physics/engines/bulletEngine.h>
#include <atta/physics/engines/noneEngine.h>

namespace atta::physics {

enum class Plane2D;
class Manager final {
  public:
    static Manager& getInstance();

    friend void startUp();
    friend void shutDown();
    friend void update(float dt);
    friend Engine::Type getEngineType();
    friend void setEngineType(Engine::Type type);
    template <typename T>
    friend std::shared_ptr<T> getEngine();

    friend Plane2D getPlane2D();
    friend void setPlane2D(Plane2D plane2D);
    friend vec3 getGravity();
    friend void setGravity(vec3 gravity);
    friend bool getShowColliders();
    friend void setShowColliders(bool showColliders);
    friend bool getShowContacts();
    friend void setShowContacts(bool showContacts);
    friend bool getShowJoints();
    friend void setShowJoints(bool showJoints);

    friend std::vector<component::EntityId> getEntityCollisions(component::EntityId eid);
    friend std::vector<RayCastHit> rayCast(vec3 begin, vec3 end, bool onlyFirst);
    friend bool areColliding(component::EntityId eid0, component::EntityId eid1);

  private:
    void startUpImpl();
    void shutDownImpl();
    void updateImpl(float dt);

    void setEngineTypeImpl(Engine::Type type);
    void setPlane2DImpl(Plane2D plane2D);
    void setGravityImpl(vec3 gravity);

    void onSimulationStateChange(event::Event& event);

    std::shared_ptr<Engine> _engine;             ///< Current physics engine
    std::shared_ptr<NoneEngine> _noneEngine;     ///< None physics engine
    std::shared_ptr<Box2DEngine> _box2DEngine;   ///< Box2D physics engine
    std::shared_ptr<BulletEngine> _bulletEngine; ///< Bullet physics engine
    Plane2D _plane2D;                            ///< Plane that is used when performing 2D simulations
    vec3 _gravity;                               ///< Gravity vector
    bool _showColliders;                         ///< UI collider rendering
    bool _showContacts;                          ///< UI contacts rendering
    bool _showJoints;                            ///< UI joints rendering
};

} // namespace atta::physics

#endif // ATTA_PHYSICS_MANAGER_H
