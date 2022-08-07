//--------------------------------------------------
// Atta Config
// config.h
// Date: 2022-03-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_CORE_CONFIG_H
#define ATTA_CORE_CONFIG_H

namespace atta {
class Config final {
  public:
    static Config& getInstance();
    static void init();

    static float getDt() { return getInstance()._dt; }
    static void setDt(float dt) { getInstance()._dt = dt; }

  private:
    void initImpl();

    float _dt;
};
} // namespace atta

#endif // ATTA_PHYSICS_PHYSICS_MANAGER_H
