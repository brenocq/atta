//--------------------------------------------------
// Atta Sensor Module
// manager.cpp
// Date: 2021-09-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/sensor/manager.h>

#include <atta/component/components/camera.h>
#include <atta/component/components/relationship.h>
#include <atta/component/components/transform.h>
#include <atta/component/interface.h>

#include <atta/event/events/createComponent.h>
#include <atta/event/events/deleteComponent.h>
#include <atta/event/events/simulationStart.h>
#include <atta/event/events/simulationStop.h>
#include <atta/event/events/uiCameraComponent.h>

#include <atta/graphics/cameras/orthographicCamera.h>
#include <atta/graphics/cameras/perspectiveCamera.h>
#include <atta/graphics/drawer.h>
#include <atta/graphics/renderers/fastRenderer.h>
#include <atta/graphics/renderers/pbrRenderer.h>
#include <atta/graphics/renderers/phongRenderer.h>

#include <atta/utils/config.h>

namespace atta::sensor {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::startUpImpl() {
    // Subscribe to simulation events
    event::subscribe<event::SimulationStart>(BIND_EVENT_FUNC(Manager::onSimulationStateChange));
    event::subscribe<event::SimulationStop>(BIND_EVENT_FUNC(Manager::onSimulationStateChange));

    // Subscribe to component events
    event::subscribe<event::CreateComponent>(BIND_EVENT_FUNC(Manager::onComponentChange));
    event::subscribe<event::DeleteComponent>(BIND_EVENT_FUNC(Manager::onComponentChange));

    // Subscribe to component ui events
    event::subscribe<event::UiCameraComponent>(BIND_EVENT_FUNC(Manager::onComponentUi));

    // Initialize sensors (component events generated before startup were not received)
    registerCameras();
}

void Manager::shutDownImpl() {
    // Destroy sensors
    unregisterCameras();
}

void Manager::updateImpl(float dt) {
    updateCameras(dt); // Render images when necessary
}

void Manager::onSimulationStateChange(event::Event& event) {
    switch (event.getType()) {
        case event::SimulationStart::type: {
            break;
        }
        case event::SimulationStop::type: {
            break;
        }
    }
}

void Manager::onComponentChange(event::Event& event) {
    switch (event.getType()) {
        case event::CreateComponent::type: {
            event::CreateComponent& e = reinterpret_cast<event::CreateComponent&>(event);
            if (e.componentId == component::TypedComponentRegistry<component::Camera>::getInstance().getId())
                registerCamera(e.entityId, static_cast<component::Camera*>(e.component));
            break;
        }
        case event::DeleteComponent::type: {
            event::DeleteComponent& e = reinterpret_cast<event::DeleteComponent&>(event);
            if (e.componentId == component::TypedComponentRegistry<component::Camera>::getInstance().getId())
                unregisterCamera(e.entityId);
            break;
        }
        default:
            LOG_WARN("sensor::Manager", "Unknown component change event");
    }
}

void Manager::onComponentUi(event::Event& event) { cameraCheckUiEvents(event); }

} // namespace atta::sensor

#include <atta/sensor/managerCamera.cpp>
