//--------------------------------------------------
// Atta UI Module
// sensorModuleWindow.h
// Date: 2022-12-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_EDITOR_WINDOWS_SENSOR_MODULE_WINDOW_H
#define ATTA_UI_EDITOR_WINDOWS_SENSOR_MODULE_WINDOW_H

#include <atta/sensor/interface.h>
#include <atta/ui/editor/window.h>

namespace atta::ui {

class SensorModuleWindow : public Window<SensorModuleWindow> {
  private:
    SensorModuleWindow();
    void renderImpl();

    friend Window<SensorModuleWindow>;
};

} // namespace atta::ui

#endif // ATTA_UI_EDITOR_WINDOWS_SENSOR_MODULE_WINDOW_H
