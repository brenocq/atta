//--------------------------------------------------
// Atta Event System
// windowResizeEvent.h
// Date: 2021-08-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_WINDOW_RESIZE_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_WINDOW_RESIZE_EVENT_H
#include <atta/eventSystem/event.h>

namespace atta
{
    class WindowResizeEvent : public EventTyped<SID("WindowResizeEvent")>
    {
    public:
        WindowResizeEvent(size_t width_, size_t height_): 
            width(width_), height(height_) {}

        size_t width, height;
    };
}

#endif// ATTA_EVENT_SYSTEM_EVENTS_WINDOW_RESIZE_H
