//--------------------------------------------------
// Atta Event Module
// createEntity.h
// Date: 2022-03-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_CREATE_ENTITY_H
#define ATTA_EVENT_EVENTS_CREATE_ENTITY_H
#include <atta/component/manager.h>
#include <atta/event/event.h>

namespace atta::event {

class CreateEntity : public EventTyped<SID("CreateEntity")> {
  public:
    component::EntityId entityId;
};

} // namespace atta::event
#endif // ATTA_EVENT_EVENTS_CREATE_ENTITY_H
