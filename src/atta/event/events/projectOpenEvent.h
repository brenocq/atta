//--------------------------------------------------
// Atta Event Module
// projectOpenEvent.h
// Date: 2021-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_PROJECT_OPEN_EVENT_H
#define ATTA_EVENT_EVENTS_PROJECT_OPEN_EVENT_H
#include <atta/event/event.h>

namespace atta
{
    class ProjectOpenEvent : public EventTyped<SID("ProjectOpenEvent")>
    {
    public:
    };
}

#endif// ATTA_EVENT_EVENTS_PROJECT_OPEN_EVENT_H
