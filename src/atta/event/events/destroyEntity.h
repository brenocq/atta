//--------------------------------------------------
// Atta Event Module
// destroyEntity.h
// Date: 2022-03-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_DESTROY_ENTITY_H
#define ATTA_EVENT_EVENTS_DESTROY_ENTITY_H
#include <atta/component/interface.h>
#include <atta/event/event.h>

namespace atta::event {

class DestroyEntity : public EventTyped<SID("DestroyEntity")> {
  public:
    component::EntityId entityId;
};

} // namespace atta::event
#endif // ATTA_EVENT_EVENTS_DESTROY_ENTITY_H
