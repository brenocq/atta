//--------------------------------------------------
// Atta Event System
// projectCloseEvent.h
// Date: 2021-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_PROJECT_CLOSE_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_PROJECT_CLOSE_EVENT_H
#include <atta/eventSystem/event.h>

namespace atta
{
    class ProjectCloseEvent : public EventTyped<SID("ProjectCloseEvent")>
    {
    public:
    };
}

#endif// ATTA_EVENT_SYSTEM_EVENTS_PROJECT_CLOSE_EVENT_H
