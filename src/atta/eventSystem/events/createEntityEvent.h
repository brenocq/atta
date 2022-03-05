//--------------------------------------------------
// Atta Event System
// createEntityEvent.h
// Date: 2022-03-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_CREATE_ENTITY_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_CREATE_ENTITY_EVENT_H
#include <atta/eventSystem/event.h>
#include <atta/componentSystem/componentManager.h>

namespace atta
{
    class CreateEntityEvent : public EventTyped<SID("CreateEntityEvent")>
    {
    public:
        EntityId entityId;
    };
}
#endif// ATTA_EVENT_SYSTEM_EVENTS_CREATE_ENTITY_EVENT_H
