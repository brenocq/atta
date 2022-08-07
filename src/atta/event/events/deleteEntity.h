//--------------------------------------------------
// Atta Event Module
// deleteEntity.h
// Date: 2022-03-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_DELETE_ENTITY_H
#define ATTA_EVENT_EVENTS_DELETE_ENTITY_H
#include <atta/component/manager.h>
#include <atta/event/event.h>

namespace atta::event {

class DeleteEntity : public EventTyped<SID("DeleteEntity")> {
  public:
    component::EntityId entityId;
};

} // namespace atta::event
#endif // ATTA_EVENT_EVENTS_DELETE_ENTITY_H
