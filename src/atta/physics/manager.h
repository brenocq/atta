//--------------------------------------------------
// Atta Physics Module
// manager.h
// Date: 2021-11-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_MANAGER_H
#define ATTA_PHYSICS_MANAGER_H

#include <atta/event/event.h>
#include <atta/physics/interface.h>

namespace atta::physics {

class Manager final {
  public:
    static Manager& getInstance();

    friend void startUp();
    friend void shutDown();
    friend void update(float dt);
    friend PhysicsEngine::Type getSelectedEngine();
    friend Plane2D getPlane2D();
    friend void setSelectedEngine(PhysicsEngine::Type type);
    friend std::shared_ptr<PhysicsEngine> getPhysicsEngine();
    friend std::vector<component::EntityId> getEntityCollisions(component::EntityId eid);
    friend std::vector<component::EntityId> rayCast(vec3 begin, vec3 end, bool onlyFirst);
    friend bool areColliding(component::EntityId eid0, component::EntityId eid1);

  private:
    void startUpImpl();
    void shutDownImpl();
    void updateImpl(float dt);
    void setSelectedEngineImpl(PhysicsEngine::Type type);

    void onSimulationStateChange(event::Event& event);

    std::shared_ptr<PhysicsEngine> _engine; ///< Current physics engine
    Plane2D _plane2D;                       ///< Plane that is used when performing 2D simulations
};

} // namespace atta::physics

#endif // ATTA_PHYSICS_MANAGER_H
