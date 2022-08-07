//--------------------------------------------------
// Atta Event Module
// windowMouseScrollEvent.h
// Date: 2021-08-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_WINDOW_MOUSE_SCROLL_EVENT_H
#define ATTA_EVENT_EVENTS_WINDOW_MOUSE_SCROLL_EVENT_H
#include <atta/event/event.h>

namespace atta
{
    class WindowMouseScrollEvent : public EventTyped<SID("WindowMouseScrollEvent")>
    {
    public:
        WindowMouseScrollEvent(float dx_, float dy_): 
            dx(dx_), dy(dy_) {}

        float dx, dy;
    };
}

#endif// ATTA_EVENT_EVENTS_WINDOW_MOUSE_SCROLL_H
