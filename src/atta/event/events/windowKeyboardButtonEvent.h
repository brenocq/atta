//--------------------------------------------------
// Atta Event Module
// windowKeyboardButtonEvent.h
// Date: 2021-08-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_WINDOW_KEYBOARD_BUTTON_EVENT_H
#define ATTA_EVENT_EVENTS_WINDOW_KEYBOARD_BUTTON_EVENT_H
#include <atta/event/event.h>

namespace atta::event {
class WindowKeyboardButtonEvent : public EventTyped<SID("WindowKeyboardButtonEvent")> {
  public:
    enum class Action { PRESS, REPEAT, RELEASE };

    WindowKeyboardButtonEvent(int key_, Action action_) : key(key_), action(action_) {}

    int key;
    Action action;
};
} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_WINDOW_KEYBOARD_BUTTON_H
