//--------------------------------------------------
// Atta Event Module
// windowCloseEvent.h
// Date: 2021-08-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_WINDOW_CLOSE_EVENT_H
#define ATTA_EVENT_EVENTS_WINDOW_CLOSE_EVENT_H
#include <atta/event/event.h>

namespace atta::event {
class WindowCloseEvent : public EventTyped<SID("WindowCloseEvent")> {
  public:
};
} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_WINDOW_CLOSE_H
