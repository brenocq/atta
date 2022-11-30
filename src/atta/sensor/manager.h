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
    friend void* sensor::getEntityCameraImGuiTexture(cmp::Entity eid);
    friend std::vector<CameraInfo>& sensor::getCameraInfos();

  private:
    // Interface
    void startUpImpl();
    void shutDownImpl();
    void updateImpl(float dt);

    // Handle events
    void onSimulationStateChange(evt::Event& event);
    void onProjectOpen(evt::Event& event);
    void onComponentChange(evt::Event& event);
    void onComponentUi(evt::Event& event);

    // Camera
    void registerCameras();
    void registerCamera(cmp::Entity entity, cmp::CameraSensor* camera);
    void unregisterCameras();
    void unregisterCamera(cmp::Entity entity);
    void updateCameras(float dt);                   ///< Render cameras when necessary
    void initializeCamera(CameraInfo& cameraInfo);  ///< Initialize camera renderer and camera
    void updateCameraModel(CameraInfo& cameraInfo); ///< Update camera poses and parameters
    void* getEntityCameraImGuiTextureImpl(cmp::Entity eid);
    void cameraCheckUiEvents(evt::Event& event);

    // Infrared
    void registerInfrareds();
    void registerInfrared(cmp::Entity entity, cmp::InfraredSensor* infrared);
    void unregisterInfrareds();
    void unregisterInfrared(cmp::Entity entity);
    void initializeInfrared(InfraredInfo& infraredInfo);
    void updateInfrareds(float dt); ///< Ray-cast sensors when necessary

    std::vector<CameraInfo> _cameras;
    std::vector<InfraredInfo> _infrareds;
};

} // namespace atta::sensor

#endif // ATTA_SENSOR_MANAGER_H
