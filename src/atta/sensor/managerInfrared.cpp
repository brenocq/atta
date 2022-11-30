//--------------------------------------------------
// Atta Sensor Module
// managerCamera.cpp
// Date: 2021-09-25
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta::sensor {

void Manager::unregisterInfrareds() { _infrareds.clear(); }
void Manager::unregisterInfrared(cmp::Entity entity) {}
void Manager::registerInfrareds() {}
void Manager::registerInfrared(cmp::Entity entity, cmp::InfraredSensor* infrared) {}
void Manager::updateInfrareds(float dt) {}

} // namespace atta::sensor
