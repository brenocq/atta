//--------------------------------------------------
// Atta Event Module
// deleteEntityEvent.h
// Date: 2022-03-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_DELETE_ENTITY_EVENT_H
#define ATTA_EVENT_EVENTS_DELETE_ENTITY_EVENT_H
#include <atta/component/componentManager.h>
#include <atta/event/event.h>

namespace atta::event {
class DeleteEntityEvent : public EventTyped<SID("DeleteEntityEvent")> {
  public:
    EntityId entityId;
};
} // namespace atta::event
#endif // ATTA_EVENT_EVENTS_DELETE_ENTITY_EVENT_H
