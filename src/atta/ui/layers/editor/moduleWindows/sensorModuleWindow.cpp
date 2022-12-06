//--------------------------------------------------
// Atta UI Module
// sensorModuleWindow.cpp
// Date: 2022-12-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/layers/editor/moduleWindows/sensorModuleWindow.h>

#include <atta/sensor/interface.h>

namespace atta::ui {

SensorModuleWindow::SensorModuleWindow() {
    setName("Sensor Module");
    _initialSize = vec2(250.0f, 300.0f);
}

void SensorModuleWindow::renderImpl() {
    bool showCameras = sensor::getShowCameras();
    bool showInfrareds = sensor::getShowInfrareds();
    if (ImGui::Checkbox("Show cameras", &showCameras))
        sensor::setShowCameras(showCameras);
    if (ImGui::Checkbox("Show infrareds", &showInfrareds))
        sensor::setShowInfrareds(showInfrareds);
}

} // namespace atta::ui
