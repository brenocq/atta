// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_UI_WINDOWS_SENSOR_MODULE_WINDOW_H
#define ATTA_UI_WINDOWS_SENSOR_MODULE_WINDOW_H

#include <atta/sensor/interface.h>
#include <atta/ui/windows/window.h>

namespace atta::ui {

class SensorModuleWindow : public Window<SensorModuleWindow> {
  private:
    SensorModuleWindow();
    void renderImpl();

    friend Window<SensorModuleWindow>;
};

} // namespace atta::ui

#endif // ATTA_UI_WINDOWS_SENSOR_MODULE_WINDOW_H
