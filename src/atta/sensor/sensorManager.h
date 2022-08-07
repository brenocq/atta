//--------------------------------------------------
// Atta Sensor Module
// sensorManager.h
// Date: 2021-09-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SENSOR_SENSOR_MANAGER_H
#define ATTA_SENSOR_SENSOR_MANAGER_H
#include <atta/component/base.h>
#include <atta/component/components/cameraComponent.h>
#include <atta/event/eventManager.h>
#include <atta/graphics/renderers/renderer.h>
#include <atta/graphics/cameras/camera.h>

namespace atta
{
    class SensorManager final
    {
    public:
        static SensorManager& getInstance();
        static void startUp();
        static void shutDown();

        /// Update executed inside atta main loop 
        /** Used to update the sensor internal model to keep it always updated (ex: the camera transform)**/
        static void update();
        ///< Update executed at each simulation step
        /** Used to update the sensor data itself (ex: camera image rendering) **/
        static void update(float dt);

        static void* getEntityCameraImGuiTexture(EntityId eid);

        // Camera
        struct CameraInfo
        {
            EntityId entity;
            CameraComponent* component;
            std::shared_ptr<Renderer> renderer;///< Camera renderer (fast, phong, PBR, ...)
            std::shared_ptr<Camera> camera;///< Camera view and projection matrices
            float lastTime;///< Used to render with the correct fps
            bool showWindow;///< If the camera details window is open or not
            std::vector<uint8_t> data;///< Camera rendered image
        };
        static std::vector<CameraInfo>& getCameraInfos();

    private:
        void startUpImpl();
        void shutDownImpl();
        void updateImpl();
        void updateImpl(float dt);

        // Handle events
        void onSimulationStateChange(Event& event);
        void onComponentChange(Event& event);
        void onComponentUI(Event& event);

        // Camera
        void unregisterCameras();
        void registerCameras();
        void registerCamera(EntityId entity, CameraComponent* camera);
        void unregisterCamera(EntityId entity);
        void updateCamerasModel();///< Update camera poses and parameeters
        void updateCameras(float dt);///< Render cameras when necessary
        void* getEntityCameraImGuiTextureImpl(EntityId eid);
        void cameraCheckUIEvents(Event& event);

        float _currTime;
        std::vector<CameraInfo> _cameras;
    };
}

#endif// ATTA_SENSOR_SENSOR_MANAGER_H
