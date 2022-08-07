//--------------------------------------------------
// Atta Event Module
// projectSaveEvent.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_PROJECT_SAVE_EVENT_H
#define ATTA_EVENT_EVENTS_PROJECT_SAVE_EVENT_H
#include <atta/event/event.h>

namespace atta
{
    class ProjectSaveEvent : public EventTyped<SID("ProjectSaveEvent")>
    {
    public:
    };
}

#endif// ATTA_EVENT_EVENTS_PROJECT_SAVE_EVENT_H
