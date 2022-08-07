//--------------------------------------------------
// Atta Event Module
// windowResizeEvent.h
// Date: 2021-08-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_WINDOW_RESIZE_EVENT_H
#define ATTA_EVENT_EVENTS_WINDOW_RESIZE_EVENT_H
#include <atta/event/event.h>

namespace atta::event {
class WindowResizeEvent : public EventTyped<SID("WindowResizeEvent")> {
  public:
    WindowResizeEvent(size_t width_, size_t height_) : width(width_), height(height_) {}

    size_t width, height;
};
} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_WINDOW_RESIZE_H
