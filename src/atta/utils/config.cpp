//--------------------------------------------------
// Atta Utils Module
// config.cpp
// Date: 2022-03-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/utils/config.h>

namespace atta {

Config& Config::getInstance() {
    static Config config;
    return config;
}

void Config::init() { getInstance().initImpl(); }
void Config::initImpl() {
    _dt = 0.015f; /// 15ms as default
    _time = 0.0f;
    _desiredStepSpeed = 1.0f;
    _realStepSpeed = 0.0f;
}

float Config::getDt() { return getInstance()._dt; }
void Config::setDt(float dt) { getInstance()._dt = dt; }
float Config::getTime() { return getInstance()._time; }
float Config::getDesiredStepSpeed() { return getInstance()._desiredStepSpeed; }
void Config::setDesiredStepSpeed(float desiredStepSpeed) { getInstance()._desiredStepSpeed = desiredStepSpeed; }
float Config::getRealStepSpeed() { return getInstance()._realStepSpeed; }
void Config::setRealStepSpeed(float realStepSpeed) { getInstance()._realStepSpeed = realStepSpeed; }

} // namespace atta
