//--------------------------------------------------
// Atta Event Module
// uiCameraComponentEvent.h
// Date: 2022-03-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_UI_CAMERA_COMPONENT_EVENT_H
#define ATTA_EVENT_EVENTS_UI_CAMERA_COMPONENT_EVENT_H
#include <atta/event/event.h>

namespace atta::event {
/// Published when the camera component "View" button is clicked
/** When the SensorManager received this event, it created a window to show the camera image **/
class UICameraComponentEvent : public EventTyped<SID("UICameraComponentEvent")> {
  public:
    enum class UIEvent { NONE = 0, VIEW_BUTTON_CLICKED };

    CameraComponent* component;
    UIEvent uiEvent;
};
} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_UI_CAMERA_COMPONENT_EVENT_H
