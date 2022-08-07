//--------------------------------------------------
// Atta Sensor Module
// manager.h
// Date: 2021-09-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SENSOR_MANAGER_H
#define ATTA_SENSOR_MANAGER_H
#include <atta/component/base.h>
#include <atta/component/components/camera.h>
#include <atta/event/eventManager.h>
#include <atta/graphics/cameras/camera.h>
#include <atta/graphics/renderers/renderer.h>

namespace atta::sensor {

class Manager final {
  public:
    static Manager& getInstance();

    // Camera
    struct CameraInfo {
        EntityId entity;
        component::Camera* component;
        std::shared_ptr<graphics::Renderer> renderer; ///< Camera renderer (fast, phong, PBR, ...)
        std::shared_ptr<graphics::Camera> camera;     ///< Camera view and projection matrices
        float lastTime;                               ///< Used to render with the correct fps
        bool showWindow;                              ///< If the camera details window is open or not
        std::vector<uint8_t> data;                    ///< Camera rendered image
    };

  private:
    // Interface
    void startUpImpl();
    void shutDownImpl();
    void updateImpl();
    void updateImpl(float dt);
    friend std::vector<Manager::CameraInfo>& getCameraInfos();

    // Handle events
    void onSimulationStateChange(Event& event);
    void onComponentChange(Event& event);
    void onComponentUi(Event& event);

    // Camera
    void unregisterCameras();
    void registerCameras();
    void registerCamera(EntityId entity, Camera* camera);
    void unregisterCamera(EntityId entity);
    void updateCamerasModel();    ///< Update camera poses and parameeters
    void updateCameras(float dt); ///< Render cameras when necessary
    void* getEntityCameraImGuiTextureImpl(EntityId eid);
    void cameraCheckUiEvents(Event& event);

    float _currTime;
    std::vector<CameraInfo> _cameras;
};

//-------------------------------//
//---------- Interface ----------//
//-------------------------------//
void startUp();
void shutDown();

/// Update executed inside atta main loop
/** Used to update the sensor internal model to keep it always updated (ex: the camera transform)**/
void update();

///< Update executed at each simulation step
/** Used to update the sensor data itself (ex: camera image rendering) **/
void update(float dt);

void* getEntityCameraImGuiTexture(EntityId eid);

std::vector<Manager::CameraInfo>& getCameraInfos();

} // namespace atta::sensor

#endif // ATTA_SENSOR_MANAGER_H
