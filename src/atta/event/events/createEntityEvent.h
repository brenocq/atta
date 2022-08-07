//--------------------------------------------------
// Atta Event Module
// createEntityEvent.h
// Date: 2022-03-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_CREATE_ENTITY_EVENT_H
#define ATTA_EVENT_EVENTS_CREATE_ENTITY_EVENT_H
#include <atta/event/event.h>
#include <atta/component/componentManager.h>

namespace atta
{
    class CreateEntityEvent : public EventTyped<SID("CreateEntityEvent")>
    {
    public:
        EntityId entityId;
    };
}
#endif// ATTA_EVENT_EVENTS_CREATE_ENTITY_EVENT_H
