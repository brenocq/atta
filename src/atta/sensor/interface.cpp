//--------------------------------------------------
// Atta Sensor Module
// interface.cpp
// Date: 2022-08-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/sensor/interface.h>
#include <atta/sensor/manager.h>

namespace atta::sensor {

void startUp() { Manager::getInstance().startUpImpl(); }

void shutDown() { Manager::getInstance().shutDownImpl(); }

void update() { Manager::getInstance().updateImpl(); }

void update(float dt) { Manager::getInstance().updateImpl(dt); }

std::vector<CameraInfo>& getCameraInfos() { return Manager::getInstance()._cameras; }

void* getEntityCameraImGuiTexture(component::EntityId eid) { return Manager::getInstance().getEntityCameraImGuiTextureImpl(eid); }

} // namespace atta::sensor
