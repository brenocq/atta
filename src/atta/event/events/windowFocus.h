//--------------------------------------------------
// Atta Event Module
// windowFocus.h
// Date: 2021-08-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_WINDOW_FOCUS_H
#define ATTA_EVENT_EVENTS_WINDOW_FOCUS_H
#include <atta/event/event.h>

namespace atta::event {

class WindowFocus : public EventTyped<SID("WindowFocus")> {
  public:
    WindowFocus(bool hasFocus_) : hasFocus(hasFocus_) {}

    bool hasFocus;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_WINDOW_FOCUS_H
