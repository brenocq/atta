//--------------------------------------------------
// Atta UI Module
// sensorDrawer.h
// Date: 2022-03-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_LAYERS_EDITOR_DRAWERS_SENSOR_DRAWER_H
#define ATTA_UI_LAYERS_EDITOR_DRAWERS_SENSOR_DRAWER_H

namespace atta::ui {

/// Drawer used to draw sensor lines
class SensorDrawer {
  public:
    void update(); ///< Update sensor lines
  private:
    void updateCameras();
};

} // namespace atta::ui

#endif // ATTA_UI_LAYERS_EDITOR_DRAWERS_SENSOR_DRAWER_H
