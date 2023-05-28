//--------------------------------------------------
// Atta Physics Module
// engine.h
// Date: 2021-11-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_ENGINES_ENGINE_H
#define ATTA_PHYSICS_ENGINES_ENGINE_H

#include <atta/component/interface.h>
#include <atta/utils/math/math.h>

namespace atta::physics {

class RayCastHit;
class Engine {
  public:
    ///< Available physics engines
    enum Type { NONE = 0, BOX2D, BULLET };

    Engine(Type type);
    virtual ~Engine() = default;
    virtual void start() = 0;
    virtual void step(float dt) = 0;
    virtual void stop() = 0;

    virtual void createRigidBody(component::Entity entity);
    virtual void deleteRigidBody(component::Entity entity);
    virtual void createColliders(component::Entity entity);
    virtual void deleteColliders(component::Entity entity);

    virtual std::vector<component::Entity> getEntityCollisions(component::Entity entity);
    virtual std::vector<RayCastHit> rayCast(vec3 begin, vec3 end, bool onlyFirst = false);
    virtual bool areColliding(component::Entity entity0, component::Entity entity1);

    /// Physics engine should update the gravity with the new value in Manager::getGravity()
    virtual void updateGravity() {}

    Type getType() const { return _type; }
    bool getRunning() const { return _running; }

  protected:
    Type _type;    ///< Physics engine type
    bool _running; ///< If physics engine is performing simulations
};

} // namespace atta::physics

#endif // ATTA_PHYSICS_ENGINES_ENGINE_H
