//--------------------------------------------------
// Atta Physics Module
// noneEngine.h
// Date: 2022-09-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_ENGINES_NONE_ENGINE_H
#define ATTA_PHYSICS_ENGINES_NONE_ENGINE_H

#include <atta/physics/engines/engine.h>

namespace atta::physics {

class NoneEngine : public Engine {
  public:
    NoneEngine();
    ~NoneEngine();

    void start() override;
    void step(float dt) override;
    void stop() override;
};

} // namespace atta::physics

#endif // ATTA_PHYSICS_ENGINES_NONE_ENGINE_H
