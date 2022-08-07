//--------------------------------------------------
// Atta Event Module
// windowMouseButtonEvent.h
// Date: 2021-08-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_WINDOW_MOUSE_BUTTON_EVENT_H
#define ATTA_EVENT_EVENTS_WINDOW_MOUSE_BUTTON_EVENT_H
#include <atta/event/event.h>

namespace atta::event {
class WindowMouseButtonEvent : public EventTyped<SID("WindowMouseButtonEvent")> {
  public:
    enum class Action { PRESS, RELEASE };

    WindowMouseButtonEvent(int button_, Action action_) : button(button_), action(action_) {}

    int button;
    Action action;
};
} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_WINDOW_MOUSE_BUTTON_H
