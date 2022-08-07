//--------------------------------------------------
// Atta Event Module
// windowFocusEvent.h
// Date: 2021-08-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_WINDOW_FOCUS_EVENT_H
#define ATTA_EVENT_EVENTS_WINDOW_FOCUS_EVENT_H
#include <atta/event/event.h>

namespace atta::event {
class WindowFocusEvent : public EventTyped<SID("WindowFocusEvent")> {
  public:
    WindowFocusEvent(bool hasFocus_) : hasFocus(hasFocus_) {}

    bool hasFocus;
};
} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_WINDOW_FOCUS_H
