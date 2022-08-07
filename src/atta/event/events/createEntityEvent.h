//--------------------------------------------------
// Atta Event Module
// createEntityEvent.h
// Date: 2022-03-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_CREATE_ENTITY_EVENT_H
#define ATTA_EVENT_EVENTS_CREATE_ENTITY_EVENT_H
#include <atta/component/componentManager.h>
#include <atta/event/event.h>

namespace atta::event {
class CreateEntityEvent : public EventTyped<SID("CreateEntityEvent")> {
  public:
    EntityId entityId;
};
} // namespace atta::event
#endif // ATTA_EVENT_EVENTS_CREATE_ENTITY_EVENT_H
