//--------------------------------------------------
// Atta Event Module
// windowMouseButton.h
// Date: 2021-08-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_WINDOW_MOUSE_BUTTON_H
#define ATTA_EVENT_EVENTS_WINDOW_MOUSE_BUTTON_H
#include <atta/event/event.h>

namespace atta::event {

class WindowMouseButton : public EventTyped<SID("WindowMouseButton")> {
  public:
    enum class Action { PRESS, RELEASE };

    WindowMouseButton(int button_, Action action_) : button(button_), action(action_) {}

    int button;
    Action action;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_WINDOW_MOUSE_BUTTON_H
