//--------------------------------------------------
// Atta Event System
// deleteComponentEvent.h
// Date: 2022-03-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_DELETE_COMPONENT_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_DELETE_COMPONENT_EVENT_H
#include <atta/eventSystem/event.h>
#include <atta/componentSystem/componentManager.h>

namespace atta
{
    class DeleteComponentEvent : public EventTyped<SID("DeleteComponentEvent")>
    {
    public:
        ComponentId componentId;
        EntityId entityId;
    };
}
#endif// ATTA_EVENT_SYSTEM_EVENTS_DELETE_COMPONENT_EVENT_H
