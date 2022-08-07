//--------------------------------------------------
// Atta Physics Module
// nullEngine.h
// Date: 2021-12-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_PHYSICS_ENGINES_NULL_ENGINE_H
#define ATTA_PHYSICS_PHYSICS_ENGINES_NULL_ENGINE_H
#include <atta/physics/physicsEngines/physicsEngine.h>

namespace atta::physics {
class NullEngine : public PhysicsEngine {
  public:
    NullEngine() : PhysicsEngine(PhysicsEngine::NULL_ENGINE) {}

    void start() override { _running = true; }
    void step(float dt) override {}
    void stop() override { _running = false; }
};
} // namespace atta::physics

#endif // ATTA_PHYSICS_PHYSICS_ENGINES_NULL_ENGINE_H
