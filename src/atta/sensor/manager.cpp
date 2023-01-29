//--------------------------------------------------
// Atta Sensor Module
// manager.cpp
// Date: 2021-09-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/sensor/manager.h>

#include <atta/component/components/cameraSensor.h>
#include <atta/component/components/relationship.h>
#include <atta/component/components/transform.h>
#include <atta/component/interface.h>

#include <atta/event/events/createComponent.h>
#include <atta/event/events/deleteComponent.h>
#include <atta/event/events/projectOpen.h>
#include <atta/event/events/simulationStart.h>
#include <atta/event/events/simulationStop.h>
#include <atta/event/events/uiCameraComponent.h>

#include <atta/graphics/cameras/orthographicCamera.h>
#include <atta/graphics/cameras/perspectiveCamera.h>
#include <atta/graphics/drawer.h>
#include <atta/graphics/renderers/fastRenderer.h>
#include <atta/graphics/renderers/pbrRenderer.h>
#include <atta/graphics/renderers/phongRenderer.h>

#include <atta/physics/interface.h>

#include <atta/utils/config.h>

#include <random>

namespace atta::sensor {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::startUpImpl() {
    // Subscribe to simulation events
    evt::subscribe<evt::SimulationStart>(BIND_EVENT_FUNC(Manager::onSimulationStateChange));
    evt::subscribe<evt::SimulationStop>(BIND_EVENT_FUNC(Manager::onSimulationStateChange));

    // Subscribe to component events
    evt::subscribe<evt::CreateComponent>(BIND_EVENT_FUNC(Manager::onComponentChange));
    evt::subscribe<evt::DeleteComponent>(BIND_EVENT_FUNC(Manager::onComponentChange));

    // Subscribe to component ui events
    evt::subscribe<evt::UiCameraComponent>(BIND_EVENT_FUNC(Manager::onComponentUi));

    // Subscribe to project events
    evt::subscribe<evt::ProjectOpen>(BIND_EVENT_FUNC(Manager::onProjectOpen));

    // Initialize sensors (component events generated before startup were not received)
    registerCameras();
    registerInfrareds();

    _showCameras = true;
    _showInfrareds = true;
}

void Manager::shutDownImpl() {
    // Destroy sensors
    unregisterCameras();
    unregisterInfrareds();
}

void Manager::updateImpl(float dt) {
    updateCameras(dt);
    updateInfrareds(dt);
}

void Manager::onSimulationStateChange(event::Event& event) {
    switch (event.getType()) {
        case event::SimulationStart::type: {
            for (CameraInfo& cameraInfo : _cameras)
                initializeCamera(cameraInfo);
            for (InfraredInfo& infraredInfo : _infrareds)
                initializeInfrared(infraredInfo);
            break;
        }
        case event::SimulationStop::type: {
            break;
        }
    }
}

void Manager::onProjectOpen(evt::Event& event) {
    // Make sure prototype sensors are not created
    unregisterCameras();
    registerCameras();

    unregisterInfrareds();
    registerInfrareds();
}

void Manager::onComponentChange(evt::Event& event) {
    switch (event.getType()) {
        case evt::CreateComponent::type: {
            evt::CreateComponent& e = reinterpret_cast<evt::CreateComponent&>(event);

            if (e.componentId == cmp::getId<cmp::CameraSensor>())
                registerCamera(e.entityId, static_cast<cmp::CameraSensor*>(e.component));
            else if (e.componentId == cmp::getId<cmp::InfraredSensor>())
                registerInfrared(e.entityId, static_cast<cmp::InfraredSensor*>(e.component));

            break;
        }
        case evt::DeleteComponent::type: {
            evt::DeleteComponent& e = reinterpret_cast<evt::DeleteComponent&>(event);

            if (e.componentId == cmp::getId<cmp::CameraSensor>())
                unregisterCamera(e.entityId);
            else if (e.componentId == cmp::getId<cmp::InfraredSensor>())
                unregisterInfrared(e.entityId);

            break;
        }
        default:
            LOG_WARN("sensor::Manager", "Unknown component change event");
    }
}

void Manager::onComponentUi(evt::Event& event) { cameraCheckUiEvents(event); }

} // namespace atta::sensor

#include <atta/sensor/managerCamera.cpp>
#include <atta/sensor/managerInfrared.cpp>
