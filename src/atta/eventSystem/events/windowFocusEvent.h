//--------------------------------------------------
// Atta Event System
// windowFocusEvent.h
// Date: 2021-08-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_WINDOW_FOCUS_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_WINDOW_FOCUS_EVENT_H
#include <atta/eventSystem/event.h>

namespace atta
{
    class WindowFocusEvent : public EventTyped<SID("WindowFocusEvent")>
    {
    public:
        WindowFocusEvent(bool hasFocus_): 
            hasFocus(hasFocus_) {}

        bool hasFocus;
    };
}

#endif// ATTA_EVENT_SYSTEM_EVENTS_WINDOW_FOCUS_H
