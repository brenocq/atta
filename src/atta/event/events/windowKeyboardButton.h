//--------------------------------------------------
// Atta Event Module
// windowKeyboardButton.h
// Date: 2021-08-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_WINDOW_KEYBOARD_BUTTON_H
#define ATTA_EVENT_EVENTS_WINDOW_KEYBOARD_BUTTON_H
#include <atta/event/event.h>

namespace atta::event {

class WindowKeyboardButton : public EventTyped<SID("WindowKeyboardButton")> {
  public:
    enum class Action { PRESS, REPEAT, RELEASE };

    WindowKeyboardButton(int key_, Action action_) : key(key_), action(action_) {}

    int key;
    Action action;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_WINDOW_KEYBOARD_BUTTON_H
