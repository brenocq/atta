//--------------------------------------------------
// Atta Event System
// deleteEntityEvent.h
// Date: 2022-03-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_DELETE_ENTITY_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_DELETE_ENTITY_EVENT_H
#include <atta/eventSystem/event.h>
#include <atta/componentSystem/componentManager.h>

namespace atta
{
    class DeleteEntityEvent : public EventTyped<SID("DeleteEntityEvent")>
    {
    public:
        EntityId entityId;
    };
}
#endif// ATTA_EVENT_SYSTEM_EVENTS_DELETE_ENTITY_EVENT_H
