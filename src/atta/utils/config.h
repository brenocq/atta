//--------------------------------------------------
// Atta Utils Module
// config.h
// Date: 2022-03-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UTILS_CONFIG_H
#define ATTA_UTILS_CONFIG_H

namespace atta {

class Atta;
class Config final {
  public:
    enum class State { IDLE = 0, RUNNING, PAUSED };

    static Config& getInstance();
    static void init();

    static State getState();
    static float getDt();
    static void setDt(float dt);
    static float getTime();
    static float getDesiredStepSpeed();
    static void setDesiredStepSpeed(float desiredStepSpeed);
    static float getRealStepSpeed();
    static void setRealStepSpeed(float realStepSpeed);

  private:
    void initImpl();

    /// Simulation state
    State _state;

    //----- Stepping -----//
    float _dt;    ///< Simulation step in seconds
    float _time;  ///< Current simulation time
    /// Desired step speed
    /** Desired speed relative to real time
     * 0.0 = step as fast as possible
     * 0.5 = step two times slower than real time
     * 1.0 = step in real time.
     * 2.0 = step two times faster than real time
     **/
    float _desiredStepSpeed;
    ///< Real step speed
    /** Real speed relative to real time
     * This variable is updated by Atta::loop
     **/
    float _realStepSpeed;
    friend Atta;
};

} // namespace atta

#endif // ATTA_PHYSICS_PHYSICS_MANAGER_H
