//--------------------------------------------------
// Atta Physics Module
// manager.h
// Date: 2021-11-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_MANAGER_H
#define ATTA_PHYSICS_MANAGER_H
#include <atta/event/event.h>
#include <atta/physics/physicsEngines/physicsEngine.h>

namespace atta::physics {

class Manager final {
  public:
    enum class Plane2D { X = 0, Y, Z };

    static Manager& getInstance();
    static void startUp();
    static void shutDown();
    static void update(float dt);
    static PhysicsEngine::Type getSelectedEngine() { return getInstance()._engine->getType(); }
    static Plane2D getPlane2D() { return getInstance()._plane2D; }
    static void setSelectedEngine(PhysicsEngine::Type type);
    static std::shared_ptr<PhysicsEngine> getPhysicsEngine() { return getInstance()._engine; }

    //---------- Queries ----------//
    static std::vector<component::EntityId> getEntityCollisions(component::EntityId eid);
    static std::vector<component::EntityId> rayCast(vec3 begin, vec3 end, bool onlyFirst = true);
    static bool areColliding(component::EntityId eid0, component::EntityId eid1);

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
