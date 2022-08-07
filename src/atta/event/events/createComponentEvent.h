//--------------------------------------------------
// Atta Event Module
// createComponentEvent.h
// Date: 2022-03-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_CREATE_COMPONENT_EVENT_H
#define ATTA_EVENT_EVENTS_CREATE_COMPONENT_EVENT_H
#include <atta/event/event.h>
#include <atta/component/componentManager.h>

namespace atta
{
    class CreateComponentEvent : public EventTyped<SID("CreateComponentEvent")>
    {
    public:
        ComponentId componentId;
        EntityId entityId;
        Component* component;
    };
}
#endif// ATTA_EVENT_EVENTS_CREATE_COMPONENT_EVENT_H
