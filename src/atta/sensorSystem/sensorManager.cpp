//--------------------------------------------------
// Atta Sensor System
// sensorManager.cpp
// Date: 2021-09-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/sensorSystem/sensorManager.h>
#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/components/transformComponent.h>
#include <atta/componentSystem/components/relationshipComponent.h>
#include <atta/eventSystem/events/simulationStartEvent.h>
#include <atta/eventSystem/events/simulationStopEvent.h>
#include <atta/eventSystem/events/createComponentEvent.h>
#include <atta/eventSystem/events/deleteComponentEvent.h>
#include <atta/eventSystem/events/uiCameraComponentEvent.h>
#include <atta/graphicsSystem/renderers/fastRenderer.h>
#include <atta/graphicsSystem/renderers/phongRenderer.h>
#include <atta/graphicsSystem/renderers/pbrRenderer.h>
#include <atta/graphicsSystem/cameras/orthographicCamera.h>
#include <atta/graphicsSystem/cameras/perspectiveCamera.h>
#include <atta/graphicsSystem/drawer.h>

namespace atta
{
    SensorManager& SensorManager::getInstance()
    {
        static SensorManager instance;
        return instance;
    }

    void SensorManager::startUp() { getInstance().startUpImpl(); }
    void SensorManager::startUpImpl()
    {
        // Subscribe to simulation events
        EventManager::subscribe<SimulationStartEvent>(BIND_EVENT_FUNC(SensorManager::onSimulationStateChange));
        EventManager::subscribe<SimulationStopEvent>(BIND_EVENT_FUNC(SensorManager::onSimulationStateChange));

        // Subscribe to component events
        EventManager::subscribe<CreateComponentEvent>(BIND_EVENT_FUNC(SensorManager::onComponentChange));
        EventManager::subscribe<DeleteComponentEvent>(BIND_EVENT_FUNC(SensorManager::onComponentChange));

        // Subscribe to component ui events
        EventManager::subscribe<UICameraComponentEvent>(BIND_EVENT_FUNC(SensorManager::onComponentUI));

        // Initialize sensors (component events generated before startup were not received)
        registerCameras();
    }

    void SensorManager::shutDown() { getInstance().shutDownImpl(); }
    void SensorManager::shutDownImpl()
    {
        // Destroy sensors
        unregisterCameras();
    }

    void SensorManager::update() { getInstance().updateImpl(); }
    void SensorManager::updateImpl()
    {
        updateCamerasModel();// Update camera pose and internal paramters
    }

    void SensorManager::update(float dt) { getInstance().updateImpl(dt); }
    void SensorManager::updateImpl(float dt)
    {
        _currTime += dt;
        updateCameras(dt);// Render images when necessary
    }

    void SensorManager::onSimulationStateChange(Event& event)
    {
        switch(event.getType())
        {
            case SimulationStartEvent::type:
                {
                    _currTime = 0;
                    break;
                }
            case SimulationStopEvent::type:
                {
                    _currTime = 0;
                    break;
                }
            default:
                {
                    LOG_WARN("SensorManager", "Unknown simulation event");
                }
        }
    }

    void SensorManager::onComponentChange(Event& event)
    {
        switch(event.getType())
        {
            case CreateComponentEvent::type:
                {
                    CreateComponentEvent& e = reinterpret_cast<CreateComponentEvent&>(event);
                    if(e.componentId == TypedComponentRegistry<CameraComponent>::getInstance().getId())
                    {
                        registerCamera(e.entityId, static_cast<CameraComponent*>(e.component));
                    }
                    break;
                }
            case DeleteComponentEvent::type:
                {
                    DeleteComponentEvent& e = reinterpret_cast<DeleteComponentEvent&>(event);
                    if(e.componentId == TypedComponentRegistry<CameraComponent>::getInstance().getId())
                    {
                        unregisterCamera(e.entityId);
                    }
                    break;
                }
            default:
                {
                    LOG_WARN("SensorManager", "Unknown component change event");
                }
        }
    }

    void SensorManager::onComponentUI(Event& event)
    {
        cameraCheckUIEvents(event);
    }
}
#include <atta/sensorSystem/sensorManagerCamera.cpp>
