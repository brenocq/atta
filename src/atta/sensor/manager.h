//--------------------------------------------------
// Atta Sensor Module
// manager.h
// Date: 2021-09-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SENSOR_MANAGER_H
#define ATTA_SENSOR_MANAGER_H

#include <atta/sensor/interface.h>

#include <atta/event/interface.h>

namespace atta::sensor {

class Manager final {
  public:
    static Manager& getInstance();

    friend void sensor::startUp();
    friend void sensor::shutDown();
    friend void sensor::update(float dt);
    friend void* sensor::getEntityCameraImGuiTexture(component::EntityId eid);
    friend std::vector<CameraInfo>& sensor::getCameraInfos();

  private:
    // Interface
    void startUpImpl();
    void shutDownImpl();
    void updateImpl(float dt);

    // Handle events
    void onSimulationStateChange(event::Event& event);
    void onComponentChange(event::Event& event);
    void onComponentUi(event::Event& event);

    // Camera
    void unregisterCameras();
    void registerCameras();
    void registerCamera(component::EntityId entity, component::Camera* camera);
    void unregisterCamera(component::EntityId entity);
    void updateCameras(float dt);                   ///< Render cameras when necessary
    void initializeCamera(CameraInfo& cameraInfo);  ///< Initialize camera renderer and camera
    void updateCameraModel(CameraInfo& cameraInfo); ///< Update camera poses and parameters
    void* getEntityCameraImGuiTextureImpl(component::EntityId eid);
    void cameraCheckUiEvents(event::Event& event);

    std::vector<CameraInfo> _cameras;
};

} // namespace atta::sensor

#endif // ATTA_SENSOR_MANAGER_H
