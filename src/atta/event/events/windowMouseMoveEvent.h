//--------------------------------------------------
// Atta Event Module
// windowMouseMoveEvent.h
// Date: 2021-08-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_WINDOW_MOUSE_MOVE_EVENT_H
#define ATTA_EVENT_EVENTS_WINDOW_MOUSE_MOVE_EVENT_H
#include <atta/event/event.h>

namespace atta
{
    class WindowMouseMoveEvent : public EventTyped<SID("WindowMouseMoveEvent")>
    {
    public:
        WindowMouseMoveEvent(float x_, float y_): 
            x(x_), y(y_) {}

        float x, y;
    };
}

#endif// ATTA_EVENT_EVENTS_WINDOW_MOUSE_MOVE_H
