//--------------------------------------------------
// Atta Event System
// projectBeforeDeserializeEvent.h
// Date: 2021-11-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_PROJECT_BEFORE_DESERIALIZE_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_PROJECT_BEFORE_DESERIALIZE_EVENT_H
#include <atta/eventSystem/event.h>

namespace atta
{
    class ProjectBeforeDeserializeEvent : public EventTyped<SID("ProjectBeforeDeserializeEvent")>
    {
    public:
    };
}

#endif// ATTA_EVENT_SYSTEM_EVENTS_PROJECT_BEFORE_DESERIALIZE_EVENT_H
