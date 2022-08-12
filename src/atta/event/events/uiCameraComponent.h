//--------------------------------------------------
// Atta Event Module
// uiCameraComponent.h
// Date: 2022-03-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_UI_CAMERA_COMPONENT_H
#define ATTA_EVENT_EVENTS_UI_CAMERA_COMPONENT_H
#include <atta/event/event.h>

namespace atta::event {

/// Published when the camera component "View" button is clicked
/** When the SensorManager received this event, it created a window to show the camera image **/
class UiCameraComponent : public EventTyped<SID("UiCameraComponent")> {
  public:
    enum class UiEvent { NONE = 0, VIEW_BUTTON_CLICKED };

    component::Camera* component;
    UiEvent uiEvent;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_UI_CAMERA_COMPONENT_H
