//--------------------------------------------------
// Atta Sensor Module
// interface.h
// Date: 2022-08-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SENSOR_INTERFACE_H
#define ATTA_SENSOR_INTERFACE_H

#include <atta/component/base.h>
#include <atta/component/components/camera.h>
#include <atta/graphics/cameras/camera.h>
#include <atta/graphics/renderers/renderer.h>

namespace atta::sensor {

// Camera
struct CameraInfo {
    component::EntityId entity;
    component::Camera* component;
    bool initialized;                             ///< If renderer/camera were initialized
    std::shared_ptr<graphics::Renderer> renderer; ///< Camera renderer (fast, phong, PBR, ...)
    std::shared_ptr<graphics::Camera> camera;     ///< Camera view and projection matrices
    float lastTime;                               ///< Used to render with the correct fps
    bool showWindow;                              ///< If the camera details window is open or not
    std::vector<uint8_t> data;                    ///< Camera rendered image
};

/// Sensor module start up
void startUp();

/// Sensor module shut down
void shutDown();

///< Update executed at each simulation step
/** Used to update the sensor data (ex: camera image rendering) **/
void update(float dt);

void* getEntityCameraImGuiTexture(component::EntityId eid);

std::vector<CameraInfo>& getCameraInfos();

} // namespace atta::sensor

#endif // ATTA_SENSOR_INTERFACE_H
