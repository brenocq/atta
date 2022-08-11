//--------------------------------------------------
// Atta Event Module
// deleteComponent.h
// Date: 2022-03-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_DELETE_COMPONENT_H
#define ATTA_EVENT_EVENTS_DELETE_COMPONENT_H
#include <atta/component/interface.h>
#include <atta/event/event.h>

namespace atta::event {

class DeleteComponent : public EventTyped<SID("DeleteComponent")> {
  public:
    component::ComponentId componentId;
    component::EntityId entityId;
};

} // namespace atta::event
#endif // ATTA_EVENT_EVENTS_DELETE_COMPONENT_H
